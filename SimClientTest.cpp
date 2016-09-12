// Sample code that calls the Contest Super Simulator. 
// There are basic things we do here:
// a) simulate calling CQ in CW in a way that propts the Simulator to answer, 
//    showing how to do it without WinKey
// b) the same in RTTY
// c) Get the simulator to generate CW sidetone showing an alternative to
//    the simulator's built-in WinKey emulation.
// d) Get the simulator's CQing station list. Useful to populate a Band Map
//
#include <string>
#include <atlbase.h>
#include <iostream>

#include "SimIntefaceHelper.h"
#include "CwSidetoneNotifyImpl.h"

namespace {
    void DoWithCOM();
    void DemoCwQso(IKnowDispatchIsManagerPtr pMgr);
    void DemoRtty(IKnowDispatchIsManagerPtr pMgr);
    int DemoSidetone(IKnowDispatchIsManagerPtr pMgr);
    void DemoGetStations(IKnowDispatchIsManagerPtr pMgr);
}

int main(int argc, char* argv[])
{
    ::CoInitialize(0);
    DoWithCOM();    // COM objects must be released BEFORE CoUninitialize

    /* A word about how the "rules of COM" affect plumbing an existing application to 
    ** interface with the Contest Super Simulator.
    **
    ** The calls to the CSS are out-of-apartment COM calls, and therefore cannot be called
    ** by code that is itself dispatched by Windows SendMessage. 
    ** What might that mean? First a word about what does work OK:
    ** Windows GUI and Timer events in most applications typically appear in DispatchEvents
    ** via PostMessage. That sequence is acceptable to COM.
    **
    ** But a special case that does not work is, say, a mouse pad driver that
    ** uses SendMessage to convert finger gestures to WM_VSCROLL,WM_HSCROLL. Putting one
    ** of these COM calls in a WM_VSCROLL handler, for example, results in code that
    ** works sometimes and not others--depending on whether the events originated in
    ** a keyboard or mouse interaction with an HWND (which works) or a finger gesture
    ** on a mouse pad (which does not).
    **
    ** One way to be sure these COM calls always work is to dedicate a WM_COMMAND handler
    ** and, wherever in the application the need arises to call CSS via COM, use PostMessage
    ** to get to the code that calls CSS.
    **
    ** A more technical, related issue is described here:
    ** http://stackoverflow.com/questions/8839195/an-outgoing-call-cannot-be-made-since-the-application-is-dispatching-an-input-sy#8845340
    */
    ::CoUninitialize();
	return 0;
}

namespace
{
    void DoWithCOM()
    {
        try
        {

            // For the interfaces named:
            //      IKnowDispatchIsManager
            //      IKnowDispatchIsCwSimulator
            //      IKnowDispatchIsRadioSimulator
            // If we used an admin install of the interfaces, we could make
            // Windows "marshall" the above interfaces..
            // 
            // But they are just IDispatch which has a built-in marshaller. Use 
            // some goop in SimInterfaceHelper.h to convince the Microsoft C++ compiler to 
            // attach its IDispatch helpers to an unaddorned IDispatch interface.

            // The simulator manager registers itself in the Windows ROT when the user runs it.
            IKnowDispatchIsManagerPtr        pMgr;
            pMgr.GetActiveObject(__uuidof(ContestSuperSimDispLib::SuperSimManager));

            char c; // place to read for cin to block for user input
            if (!pMgr)
            {
                std::cout << "SuperSimManager not active" << std::endl;
                std::cin.read(&c, 1);
                return;
            }

            // The manager object provides access to the virtual radio objects.
            short radioIdx = 0; /* zero-based index of radio number */
            // The simulator launches some CPU-intensive work for every radioIdx. 
            // ...and the simulator offers no way to shut down a virtual radio, once started this way.
            IKnowDispatchIsRadioSimulatorPtr pRadio =  pMgr->GetSimulatorForRadio(
                                                            radioIdx   
                                                            );

            // The simulator initializes its virtual radios on frequencies of its own choosing. See what that is:
            std::cout << "Got radio of frequency: " << pRadio->CenterFrequency << " KHz. ENTER to continue" << std::endl;
            std::cin.read(&c, 1);

            int yesno = DemoSidetone(pMgr);
            if (yesno == IDNO)
            {
                pMgr->GetCwSidetoneGenerator(0); // make simulator let go of our notifier
                return;
            }

            DemoCwQso(pMgr);

            // Why does this demo switch back and forth between using cout/cin and 
            // MessageBox?
            // The distinction is important because we called CoInitialize(0);
            // back in the first line in main) and we expect callbacks.
            // We won't get any COM callbacks while blocked reading cin. But we
            // will if we're in MessageBox. (this is how COM does single threaded apartments)
            // That difference is important to the simulator for only one scenario:
            // when you pass a non-null argument to GetCwSidetoneGenerator()
            // The object you pass will only get called through a Windows message
            yesno = ::MessageBox(0, "Try RTTY?", "SimClientTest", MB_YESNO);

            if (yesno == IDYES)
                DemoRtty(pMgr);

            DemoGetStations(pMgr);

        }
        catch (const std::exception &e)
        {
            std::cerr << "Caught exception " << e.what() << std::endl;
        }
        catch (const _com_error &e)
        {
            std::cerr << "Caught COM exception " << e.ErrorMessage() << std::endl;
        }
    }

    /* This programming sequence is an alternative to using the simulator's
    ** WinKey emulation.    */
    void DemoCwQso(IKnowDispatchIsManagerPtr pMgr)
    {
            IKnowDispatchIsRadioSimulatorPtr pRadio =  pMgr->GetSimulatorForRadio(0);

             // see if we can get the simulator to answer us
            _bstr_t msg("CQ TEST DE W5XD");

            // Demonstrate the timing sequence.
            CComPtr<MyNotifier>pNotify = new MyNotifier(
                // this callback happens when the CQ finishes..
                // Will ALSO get call backs for WinKey-initiated message finished...
                // Client should choose one or the other
                    [pRadio, msg]() {
                    pRadio->MessageCompletedNow("CW", msg); // tell the simulator our CQ is over
                });

            IKnowDispatchIsCwSimulatorPtr pCwSidetone = pMgr->GetCwSidetoneGenerator(pNotify);
            pCwSidetone->PutWPM(25); // Lets do a typical contest CQ speed
            short sidetoneMask = 3;
            for (;;)
            {
                pRadio->MessageStartedNow();
                pCwSidetone->QueueToTransmitText(msg);
                pCwSidetone->PutHeadphoneMask(sidetoneMask); // The HeadphoneMask property is supported only CSS 1.8 and later
                sidetoneMask = (sidetoneMask % 3) + 1;
                int yesno = ::MessageBox(0, "CQ again? No to Exit", "SimClientTest", MB_YESNO);
                if (yesno == IDNO)
                    break;
            }
   }

    void DemoRtty(IKnowDispatchIsManagerPtr pMgr)
    {
        IKnowDispatchIsRadioSimulatorPtr pRadio =  pMgr->GetSimulatorForRadio(0);

        // tune the virtual radio to RTTY
        // This demo would work on the simulator's default frequency, but
        // this one is a more conventional RTTY frequency.
        // SetListenFrequency is the key for "tuning" the simulator through its
        // virtual bands on CW as well as RTTY.
        pRadio->SetListenFrequency(7080, "RY");

            // see if we can get the simulator to answer us
        _bstr_t msg("CQ TEST DE W5XD");

        /* the drill of calling MessageStartedNow and MessageCompletedNow is shown for RTTY,
        ** but can be used on CW as an alternative to WinKey*/
        for (;;)
        {
            pRadio->MessageStartedNow();
            bool stop = ::MessageBox(0, 
                "Simulator thinks you are sending RTTY now.\r\n OK to end the CQ (or cancel)", 
                "SimClientTest", MB_YESNOCANCEL) != IDYES;
            if (stop)
                break;
            pRadio->MessageCompletedNow("RY", msg);  // we call CQ in RTTY mode...the simulator may answer
            stop = ::MessageBox(0, "Again?", "SimClientTest", MB_YESNO) != IDYES;
        }
    }

    // an alternative to just using the simulator's WinKey emulation
    int DemoSidetone(IKnowDispatchIsManagerPtr pMgr)
    {
            // Nothing here but sidetone generation
            // Client of simulator can call its winkey emulation or call on
            // its ICwSimulator, but its redundant to do both.
            IKnowDispatchIsRadioSimulatorPtr pRadio =  pMgr->GetSimulatorForRadio(0);
            CComPtr<MyNotifier>pNotify = new MyNotifier();

            IKnowDispatchIsCwSimulatorPtr pCwSidetone =
                pMgr->GetCwSidetoneGenerator(pNotify);

            if (!pCwSidetone)
            {
                std::cout << "GetCwSidetoneGenerate returned empty!" << std::endl;
                return IDNO;
            }

            pCwSidetone->PutWPM(25);
            pCwSidetone->QueueToTransmitText("test de w5xd");
            pCwSidetone->QueueWpmChange(35);
            pCwSidetone->QueueToTransmitText("test test w5xd");
            int yesno = ::MessageBox(0, "Yes to continue, No to Exit", "SimClientTest", MB_YESNO);
            pCwSidetone->AbortTransmission(); // if you OK fast enough, the transmission in progress will be aborted
            return yesno;
    }

    void DemoGetStations(IKnowDispatchIsManagerPtr pMgr)
    {
        /* Client might, for example, populate a Band Map with the info it gets here    */
        short radioNumber = 0;
        // the simulator will launch some CPU-intensive tasks on GetSimulatorForRadio.
        // So only ask it for radios that you want to pay the price for simulation.
        // The simulator does not offer a way to shut down a simulated radio once started.
        IKnowDispatchIsRadioSimulatorPtr pRadio = pMgr->GetSimulatorForRadio(radioNumber);
        if (pRadio)
        {
            IKnowDispatchIsStationListPtr pStations = pRadio->GetSimulatedStations();
            int count = pStations->Count;
            for (int i = 0; i < count; i++)
            {
                std::cout << "Contestant " << static_cast<char *>(pStations->GetCallsign(i)) <<
                    " " << static_cast<char *>(pStations->GetModulationMode(i)) <<
                    " " << static_cast<char *>(pStations->GetSpOrCq(i)) <<
                    " " << pStations->GetFreqKhz(i) << std::endl;
            }
        }
    }
}
