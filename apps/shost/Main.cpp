#include "MainComponent.h"

//==============================================================================
class SimpleHost : public juce::JUCEApplication {
public:
    //==============================================================================
    SimpleHost() {}

    const juce::String getApplicationName() override { return JUCE_APPLICATION_NAME_STRING; }

    const juce::String getApplicationVersion() override { return JUCE_APPLICATION_VERSION_STRING; }

    bool moreThanOneInstanceAllowed() override { return true; }

    //==============================================================================
    void initialise(const juce::String &commandLine) override {
        juce::ignoreUnused(commandLine);

        mainWindow.reset(new MainWindow(getApplicationName()));
    }

    void shutdown() override {
        mainWindow = nullptr; // (deletes our window)
    }

    //==============================================================================
    void systemRequestedQuit() override {
        quit();
    }

    void anotherInstanceStarted(const juce::String &commandLine) override {
        juce::ignoreUnused(commandLine);
    }

    //==============================================================================
    /*
        This class implements the desktop window that contains an instance of
        our MainComponent class.
    */
    class MainWindow : public juce::DocumentWindow {
    public:
        explicit MainWindow(juce::String name)
            : DocumentWindow(name,
                             juce::Desktop::getInstance().getDefaultLookAndFeel()
                                 .findColour(ResizableWindow::backgroundColourId),
                             DocumentWindow::allButtons) {
            setUsingNativeTitleBar(true);
            setContentOwned(new MainComponent(), true);

#if JUCE_IOS || JUCE_ANDROID
            setFullScreen (true);
#else
            setResizable(true, true);
            centreWithSize(getWidth(), getHeight());
#endif
            setVisible(true);
        }

        void closeButtonPressed() override {
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    std::unique_ptr<MainWindow> mainWindow;
};

START_JUCE_APPLICATION (SimpleHost)
