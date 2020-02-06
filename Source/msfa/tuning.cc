#include "tuning.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <math.h>
#include <sstream>
#include <vector>


struct StandardTuning : public TuningState {
    StandardTuning() {
        const int base = 50857777;  // (1 << 24) * (log(440) / log(2) - 69/12) 
        const int step = (1 << 24) / 12;
        for( int mn = 0; mn < 128; ++mn )
        {
            auto res = base + step * mn;
            current_logfreq_table_[mn] = res;
        }
    }
    
    virtual int32_t midinote_to_logfreq(int midinote) override {
        return current_logfreq_table_[midinote];
    }
    int current_logfreq_table_[128];
};


std::shared_ptr<TuningState> createStandardTuning()
{
    return std::make_shared<StandardTuning>();
}


// This code is heavily based on the surge tuning implementation
struct Tone
{
   typedef enum Type
   {
      kToneCents,
      kToneRatio
   } Type;

   Type type;
   float cents;
   int ratio_d, ratio_n;
   std::string stringRep;
   float floatValue;

   Tone() : type(kToneRatio), cents(0), ratio_d(1), ratio_n(1), stringRep("1/1"), floatValue(1.0)
   {
   }
};

struct Scale
{
   std::string name;
   std::string description;
   std::string rawText;
   int count;
   std::vector<Tone> tones;

   Scale() : name("empty scale"), description(""), rawText(""), count(0)
   {
   }

   bool isValid() const;

   static Scale evenTemperament12NoteScale();
};

struct KeyboardMapping
{
   bool isValid;
   bool isStandardMapping;
   int count;
   int firstMidi, lastMidi;
   int middleNote;
   int tuningConstantNote;
   float tuningFrequency;
   int octaveDegrees;
   std::vector<int> keys; // rather than an 'x' we use a '-1' for skipped keys

   std::string rawText;
   std::string name;
   
   KeyboardMapping() : isValid(true),
                       isStandardMapping(true),
                       count(12),
                       firstMidi(0),
                       lastMidi(127),
                       middleNote(60),
                       tuningConstantNote(60),
                       tuningFrequency(8.175798915 * 32),
                       octaveDegrees(12),
                       rawText( "" ),
                       name( "" )
      {
         for( int i=0; i<12; ++i )
            keys.push_back(i);
      }

   static KeyboardMapping tuneA69To(double freq);
};


Scale parseSCLData(const std::string &sclContents);
KeyboardMapping parseKBMData(const std::string &kbmContents);


Scale scaleFromStream(std::istream &inf)
{
   std::string line;
   const int read_header = 0, read_count = 1, read_note = 2;
   int state = read_header;

   Scale res;
   std::ostringstream rawOSS;
   while (std::getline(inf, line))
   {
      rawOSS << line << "\n";
      if (line[0] == '!')
      {
         continue;
      }
      switch (state)
      {
      case read_header:
         res.description = line;
         state = read_count;
         break;
      case read_count:
         res.count = atoi(line.c_str());
         state = read_note;
         break;
      case read_note:
         Tone t;
         t.stringRep = line;
         if (line.find(".") != std::string::npos)
         {
            t.type = Tone::kToneCents;
            t.cents = atof(line.c_str());
         }
         else
         {
            t.type = Tone::kToneRatio;
            auto slashPos = line.find("/");
            if (slashPos == std::string::npos)
            {
               t.ratio_n = atoi(line.c_str());
               t.ratio_d = 1;
            }
            else
            {
               t.ratio_n = atoi(line.substr(0, slashPos).c_str());
               t.ratio_d = atoi(line.substr(slashPos + 1).c_str());
            }

            // 2^(cents/1200) = n/d
            // cents = 1200 * log(n/d) / log(2)
            
            t.cents = 1200 * log(1.0 * t.ratio_n/t.ratio_d) / log(2.0);
         }
         t.floatValue = t.cents / 1200.0 + 1.0;
         res.tones.push_back(t);

         break;
      }
   }

   res.rawText = rawOSS.str();
   return res;
}

Scale readSCLFile(std::string fname)
{
   std::ifstream inf;
   inf.open(fname);
   if (!inf.is_open())
   {
      return Scale();
   }

   auto res = scaleFromStream(inf);
   res.name = fname;
   return res;
}

Scale parseSCLData(const std::string &d)
{
    std::istringstream iss(d);
    auto res = scaleFromStream(iss);
    res.name = "Scale from Patch";
    return res;
}

Scale Scale::evenTemperament12NoteScale()
{
   auto data = R"SCL(! even.scl
!
12 note even temprament
 12
!
 100.0
 200.0
 300.0
 400.0
 500.0
 600.0
 700.0
 800.0
 900.0
 1000.0
 1100.0
 2/1
)SCL";
   return parseSCLData(data);
}

KeyboardMapping keyboardMappingFromStream(std::istream &inf)
{
   std::string line;
   const int read_header = 0, read_count = 1, read_note = 2;

   KeyboardMapping res;
   std::ostringstream rawOSS;
   res.isStandardMapping = false;
   res.keys.clear();

   enum parsePosition {
      map_size = 0,
      first_midi,
      last_midi,
      middle,
      reference,
      freq,
      degree,
      keys
   };
   parsePosition state = map_size;
   
   while (std::getline(inf, line))
   {
      rawOSS << line << "\n";
      if (line[0] == '!')
      {
         continue;
      }

      if( line == "x" ) line = "-1";
      
      int i = std::atoi(line.c_str());
      float v = std::atof(line.c_str());

      switch (state)
      {
      case map_size:
         res.count = i;
         break;
      case first_midi:
         res.firstMidi = i;
         break;
      case last_midi:
         res.lastMidi = i;
         break;
      case middle:
         res.middleNote = i;
         break;
      case reference:
         res.tuningConstantNote = i;
         break;
      case freq:
         res.tuningFrequency = v;
         break;
      case degree:
         res.octaveDegrees = i;
         break;
      case keys:
         res.keys.push_back(i);
         break;
      }
      if( state != keys ) state = (parsePosition)(state + 1);
   }

   res.rawText = rawOSS.str();
   return res;
}

KeyboardMapping readKBMFile(std::string fname)
{
   std::ifstream inf;
   inf.open(fname);
   if (!inf.is_open())
   {
      return KeyboardMapping();
   }

   auto res = keyboardMappingFromStream(inf);
   res.name = fname;
   return res;
}

KeyboardMapping parseKBMData(const std::string &d)
{
    std::istringstream iss(d);
    auto res = keyboardMappingFromStream(iss);
    res.name = "Mapping from Patch";
    return res;
}

bool Scale::isValid() const
{
   if (count <= 0)
      return false;

   // TODO check more things maybe...
   return true;
}

KeyboardMapping KeyboardMapping::tuneA69To(double freq)
{
   // There's a couple of ways to do this but since I want it to stream I will syntheitcally create
   // a KBM file
   std::ostringstream oss;
   oss << R"KBM(! Dexed Synthetic Keyboard Tuning to Retune A69
!
! Map Size
0
! First note
0
! Last note
127
! First mapping
60
! Reference Note
69
! Reference Freqency
)KBM" << freq << R"KBM(
! Scale Degree
0
! Mapping)KBM";
   return parseKBMData( oss.str() );
}

struct SCLAndKBMTuningState : public TuningState {
    virtual int32_t midinote_to_logfreq(int midinote) override {
        return current_logfreq_table_[midinote];
    }
    int current_logfreq_table_[128];

    bool retuneTo(const Scale& s) {
        return retuneTo( s, KeyboardMapping() );
    }

    bool retuneTo(const KeyboardMapping &k) {
        return retuneTo( Scale(), k );
    }

    // Again this is basically the modified surge implementation
    bool retuneTo( const Scale &s, const KeyboardMapping &k ) {
        if (!s.isValid())
            return false;

        float cp;
        if( k.isStandardMapping )
            cp = 32;
        else
            cp = k.tuningFrequency / 8.175798915;
        
        float pitches[512];
        int posPitch0 = 256 + k.tuningConstantNote;
        int posScale0 = 256 + k.middleNote;
        float pitchMod = log(cp)/log(2) - 1;

        int scalePositionOfStartNote = 0;
        int scalePositionOfTuningNote = k.tuningConstantNote - k.middleNote;
        if( k.count > 0 )
            scalePositionOfTuningNote = k.keys[scalePositionOfTuningNote];

        float tuningCenterPitchOffset;
        if( scalePositionOfTuningNote == 0 )
            tuningCenterPitchOffset = 0;
        else
            tuningCenterPitchOffset = s.tones[scalePositionOfTuningNote-1].floatValue - 1.0;

        pitches[posPitch0] = 1.0;
        float table_pitch[512];
        
        for (int i=0; i<512; ++i)
        {
            // TODO: ScaleCenter and PitchCenter are now two different notes.
            int distanceFromPitch0 = i - posPitch0;
            int distanceFromScale0 = i - posScale0;
            
            if( distanceFromPitch0 == 0 )
            {
                table_pitch[i] = pow( 2.0, pitches[i] + pitchMod );
#if DEBUG_SCALES
                if( i > 296 && i < 340 )
                    std::cout << "PITCH: i=" << i << " n=" << i - 256 
                              << " p=" << pitches[i]
                              << " tp=" << table_pitch[i]
                              << " fr=" << table_pitch[i] * 8.175798915
                              << std::endl;
#endif           
            }
            else 
            {
                /*
                  We used to have this which assumed 1-12
                  Now we have our note number, our distance from the 
                  center note, and the key remapping
                  int rounds = (distanceFromScale0-1) / s.count;
                  int thisRound = (distanceFromScale0-1) % s.count;
                */
                
                int rounds;
                int thisRound;
                int disable = false;
                if( k.isStandardMapping || ( k.count == 0 ) )
                {
                    rounds = (distanceFromScale0-1) / s.count;
                    thisRound = (distanceFromScale0-1) % s.count;
                }
                else
                {
                    /*
                    ** Now we have this situation. We are at note i so we
                    ** are m away from the center note which is distanceFromScale0
                    **
                    ** If we mod that by the mapping size we know which note we are on
                    */
                    int mappingKey = distanceFromScale0 % k.count;
                    if( mappingKey < 0 )
                        mappingKey += k.count;
                    int cm = k.keys[mappingKey];
                    int push = 0;
                    if( cm < 0 )
                    {
                        disable = true;
                    }
                    else
                    {
                        push = mappingKey - cm;
                    }
                    rounds = (distanceFromScale0 - push - 1) / s.count;
                    thisRound = (distanceFromScale0 - push - 1) % s.count;
#ifdef DEBUG_SCALES
                    if( i > 296 && i < 340 )
                        std::cout << "MAPPING n=" << i - 256 << " pushes ds0=" << distanceFromScale0 << " cmc=" << k.count << " tr=" << thisRound << " r=" << rounds << " mk=" << mappingKey << " cm=" << cm << " push=" << push << " dis=" << disable << " mk-p-1=" << mappingKey - push - 1 << std::endl;
#endif
                    
                    
                }
                
                if( thisRound < 0 )
                {
                    thisRound += s.count;
                    rounds -= 1;
                }
                float mul = pow( s.tones[s.count-1].floatValue, rounds);
                if( disable )
                    pitches[i] = 0;
                else
                    pitches[i] = s.tones[thisRound].floatValue + rounds * (s.tones[s.count - 1].floatValue - 1.0) - tuningCenterPitchOffset;
                
                float otp = table_pitch[i];
                table_pitch[i] = pow( 2.0, pitches[i] + pitchMod );
                
#if DEBUG_SCALES
                if( i > 296 && i < 340 )
                    std::cout << "PITCH: i=" << i << " n=" << i - 256
                              << " ds0=" << distanceFromScale0 
                              << " dp0=" << distanceFromPitch0
                              << " r=" << rounds << " t=" << thisRound
                              << " p=" << pitches[i]
                              << " t=" << s.tones[thisRound].floatValue << " " << s.tones[thisRound ]
                              << " dis=" << disable
                              << " tp=" << table_pitch[i]
                              << " fr=" << table_pitch[i] * 8.175798915
                              << " otp=" << otp
                              << " tcpo=" << tuningCenterPitchOffset
                              << " diff=" << table_pitch[i] - otp
                        
                        //<< " l2p=" << log(otp)/log(2.0)
                        //<< " l2p-p=" << log(otp)/log(2.0) - pitches[i] - rounds - 3
                              << std::endl;
#endif           
            }
        }
        
        // OK so now table_pitch is the pitch where 1 -> 8.17 and 32 -> 261 and it doubles. It is also offset by 256. So
        for( int i=0; i<128; ++i )
        {
            double tp = table_pitch[i + 256 ];
            double freq = tp * 8.175798915;
            double flog = log(freq) / log(2.0);
            int res = (int)( ( 1 << 24 ) * flog );
            
            current_logfreq_table_[i] = res;
        }
        
        return true;
    }

};

std::shared_ptr<TuningState> createTuningFromSCLData( const std::string &scl )
{
    auto k = parseSCLData(scl);
    auto res = std::make_shared<SCLAndKBMTuningState>();
    res->retuneTo(k);
    return res;
}

std::shared_ptr<TuningState> createTuningFromKBMData( const std::string &kbm )
{
    auto k = parseKBMData(kbm);
    auto res = std::make_shared<SCLAndKBMTuningState>();
    res->retuneTo(Scale::evenTemperament12NoteScale(), k);
    return res;
}

std::shared_ptr<TuningState> createTuningFromSCLAndKBMData( const std::string &sclData, const std::string &kbmData )
{
    auto s = parseSCLData(sclData);
    auto k = parseKBMData(kbmData);
    auto res = std::make_shared<SCLAndKBMTuningState>();
    res->retuneTo(s,k);
    return res;
}
