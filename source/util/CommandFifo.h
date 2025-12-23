
#pragma once

#include <juce_core/juce_core.h>

// CommandFifo.h
// We want to send type-erased commands to the audio thread, but we also
// want those commands to contain move-only resources, so that we can
// construct resources on the gui thread, and then transfer ownership
// cheaply to the audio thread. We can't do this with std::function
// because it enforces that functions are copy-constructible.
// Therefore, we use a very simple templated type-eraser here.
template <typename Proc>
struct Command {
    virtual ~Command() noexcept = default;
    virtual void run(Proc& proc) = 0;
};

template <typename Proc, typename Func>
class TemplateCommand : public Command<Proc>, private Func {
public:
    template <typename FuncPrime>
    explicit TemplateCommand(FuncPrime&& funcPrime) : Func(std::forward<FuncPrime>(funcPrime)) {
    }
    void run(Proc& proc) override { (*this) (proc); }
};

template <typename Proc>
class CommandFifo final {
public:
    explicit CommandFifo(int size) : buffer((size_t)size), abstractFifo(size) {
    }

    CommandFifo() : CommandFifo(1024) {
    }

    template <typename Item>
    void push(Item&& item) noexcept {
        auto command = makeCommand(std::forward<Item>(item));

        abstractFifo.write(1).forEach([&](int index)
            {
                buffer[size_t(index)] = std::move(command);
            });
    }

    void call(Proc& proc) {
        abstractFifo.read(abstractFifo.getNumReady()).forEach([&](int index) {
            buffer[size_t(index)]->run(proc);
        });
    }

    void reset() {
        abstractFifo.reset();
    }
private:
    template <typename Func>
    static std::unique_ptr<Command<Proc>> makeCommand(Func&& func) {
        using Decayed = typename std::decay<Func>::type;
        return std::make_unique<TemplateCommand<Proc, Decayed>>(std::forward<Func>(func));
    }

    std::vector<std::unique_ptr<Command<Proc>>> buffer;
    juce::AbstractFifo abstractFifo;
};