#include "PluginProcessor.h"
#include "parameter/Model.h"

int DexedAudioProcessor::tuningTranspositionShift() {
    int transpose = activeProgram[IDs::transpose.pos];
    if( synthTuningState->is_standard_tuning() || ! controllers.transpose12AsScale )
        return transpose - 24;
    else {
        int d144 = transpose;
        if( d144 % 12 == 0 ) {
            int oct = (d144 - 24) / 12;
            int res = oct * synthTuningState->scale_length();
            return res;
        }
        else
            return transpose - 24;
    }
}

void DexedAudioProcessor::resetTuning(std::shared_ptr<TuningState> t) {
    synthTuningState = t;
    synthTuningStateLast = t;
    for( int i=0; i<MAX_ACTIVE_NOTES; ++i )
        if( voices[i].dx7_note != nullptr )
            voices[i].dx7_note->tuning_state_ = synthTuningState;
}

void DexedAudioProcessor::retuneToStandard() {
    currentSCLData = "";
    currentKBMData = "";
    resetTuning(createStandardTuning());
}

void DexedAudioProcessor::applySCLTuning() {
    tuningFileChooser = std::make_unique<FileChooser>( "Please select a scale (.scl) file.", File(), "*.scl" );

    // on validation error, the alert callback re-opens the chooser to enforce the proper selection
    auto retry = [this](int) { applySCLTuning(); };

    tuningFileChooser->launchAsync(FileBrowserComponent::openMode | FileBrowserComponent::canSelectFiles,
        [this, retry](const FileChooser& fc) {
            if (fc.getResults().isEmpty())
                // User cancelled
                return;
            File s = fc.getResult();

            // enforce file extenstion ''.scl''.
            // (reason: the extension ''.scl'' is mandatory according to
            // ''https://www.huygens-fokker.org/scala/scl_format.html''
            if (s.getFileExtension() != ".scl") {
                AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Invalid file type!", "Only files with the \".scl\" extension (in lowercase!) are allowed.", {}, nullptr, ModalCallbackFunction::create(retry));
                return;
            }

            // enforce to select file below the max limit16KB sized files
            if (s.getSize() > MAX_SCL_KBM_FILE_SIZE) {
                std::string msg;
                msg = "File size exceeded the maximum limit of " + std::to_string(MAX_SCL_KBM_FILE_SIZE) + " bytes.";
                AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "File size error!", msg, {}, nullptr, ModalCallbackFunction::create(retry));
                return;
            }

            // enforce to select non-empty file
            if (s.getSize() == 0) {
                std::string msg;
                msg = "File is empty.";
                AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "File size error!", msg, {}, nullptr, ModalCallbackFunction::create(retry));
                return;
            }

            // try to apply the SCL file
            applySCLTuning(s);
        });
}

void DexedAudioProcessor::applySCLTuning(File s) {
    std::string sclcontents = s.loadFileAsString().toStdString();
    applySCLTuning(sclcontents);
}

void DexedAudioProcessor::applySCLTuning(std::string sclcontents) {
    if( currentKBMData.size() < 1 ) {
        auto t = createTuningFromSCLData( sclcontents );
        if (t != nullptr) {
            resetTuning(t); // update tuning
            currentSCLData = sclcontents; // remember this SCL data
            synthTuningStateLast = t; // remember this whole state as a "last good working state"
        }
        else {
            resetTuning(synthTuningStateLast); // revert to the "last good working state"
        }
    }
    else {
        auto t = createTuningFromSCLAndKBMData( sclcontents, currentKBMData );
        if (t != nullptr) {
            resetTuning(t); // update tuning
            currentSCLData = sclcontents; // remember this SCL data
            synthTuningStateLast = t; // remember this whole state as a "last good working state"
        }
        else {
            resetTuning(synthTuningStateLast); // revert to the "last good working state"
        }
    }
}

void DexedAudioProcessor::applyKBMMapping() {
    tuningFileChooser = std::make_unique<FileChooser>( "Please select a keyboard map (.kbm) file.", File(), "*.kbm" );

    // on validation error, the alert callback re-opens the chooser to enforce the proper selection
    auto retry = [this](int) { applyKBMMapping(); };

    tuningFileChooser->launchAsync(FileBrowserComponent::openMode | FileBrowserComponent::canSelectFiles,
        [this, retry](const FileChooser& fc) {
            if (fc.getResults().isEmpty())
                return; // User cancelled
            File s = fc.getResult();

            // enforce file extenstion ''.kbm''.
            // (reason: the extension ''.kbm'' is mandatory according to
            // ''https://www.huygens-fokker.org/scala/scl_format.html''
            if (s.getFileExtension() != ".kbm") {
                AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Invalid file type!", "Only files with the \".kbm\" extension (in lowercase!) are allowed.", {}, nullptr, ModalCallbackFunction::create(retry));
                return;
            }

            // enforce to select file below the max limit16KB sized files
            if (s.getSize() > MAX_SCL_KBM_FILE_SIZE) {
                std::string msg;
                msg = "File size exceeded the maximum limit of " + std::to_string(MAX_SCL_KBM_FILE_SIZE) + " bytes.";
                AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "File size error!", msg, {}, nullptr, ModalCallbackFunction::create(retry));
                return;
            }

            // enforce to select non-empty file
            if (s.getSize() == 0) {
                std::string msg;
                msg = "File is empty.";
                AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "File size error!", msg, {}, nullptr, ModalCallbackFunction::create(retry));
                return;
            }

            // try to apply KBM mapping
            applyKBMMapping(s);
        });
}

void DexedAudioProcessor::applyKBMMapping( File s ) {
    std::string kbmcontents = s.loadFileAsString().toStdString();
    applyKBMMapping(kbmcontents);
}

void DexedAudioProcessor::applyKBMMapping(std::string kbmcontents) {
    if( currentSCLData.size() < 1 ) {
        auto t = createTuningFromKBMData(kbmcontents);
        if (t != nullptr) {
            resetTuning(t); // update tuning
            currentKBMData = kbmcontents; // remember this KBM data
            synthTuningStateLast = t; // remember this whole state as a "last good working state"
        } else {
            resetTuning(synthTuningStateLast); // revert to the "last good working state"
        }
    } else {
        auto t = createTuningFromSCLAndKBMData( currentSCLData, kbmcontents );
        if (t != nullptr) {
            resetTuning(t); // update tuning
            currentKBMData = kbmcontents; // remember this KBM data
            synthTuningStateLast = t; // remember this whole state as a "last good working state"
        } else {
            resetTuning(synthTuningStateLast); // revert to "last good working state"
        }
    }
}
