// Sample code that calls the Contest Super Simulator. 
// There are three basic things we do here:
// a) simulate calling CQ in CW in a way that propts the Simulator to answer
// b) the same in RTTY
// c) Get the simulator to generate CW sidetone showing an alternative to
// the simulator's built-in WinKey emulation.
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
}

int main(int argc, char* argv[])
{
    ::CoInitialize(0);
    DoWithCOM();    // COM objects must be released BEFORE CoUninitialize
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
            // But they are just IDispatch which has a built-in marshaller. Sowe just use 
            // some goop in SimInterfaceHelper.h to convince the Microsoft C++ compiler to 
            // attach its IDispatch helpers to an unaddorned IDispatch interface.

            // The simulator manager registers itself in the ROT when the user runs it.
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
            IKnowDispatchIsRadioSimulatorPtr pRadio =  pMgr->GetSimulatorForRadio(
                                                            0   /* zero-based index of radio number */
                                                            );

            // The simulator puts its virtual radios on frequencies of its own choosing. See what that is:
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
            // The distinction is important because we called CoInitialize(0); (way
            // back in the first line in main).
            // We won't get any COM callbacks while stuck reading cin. But we
            // will if we're in MessageBox. 
            // That difference is important to the simulator for only one scenario:
            // when you pass a non-null argument to GetCwSidetoneGenerator()
            // The object you pass will only get called through a Windows message
            yesno = ::MessageBox(0, "Try RTTY?", "SimClientTest", MB_YESNO);

            if (yesno == IDYES)
                DemoRtty(pMgr);

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

    void DemoCwQso(IKnowDispatchIsManagerPtr pMgr)
    {
            IKnowDispatchIsRadioSimulatorPtr pRadio =  pMgr->GetSimulatorForRadio(0);

             // see if we can get the simulator to answer us
            _bstr_t msg("CQ TEST DE W5XD");

            // Demonstrate the timing sequence.
            CComPtr<MyNotifier>pNotify = new MyNotifier(
                // this callback happens when the CQ finishes..
                // Will ALSO get call backs for WinKey-initiated message finished...
                // should not support both.
                    [pRadio, msg]() {
                    pRadio->MessageCompletedNow("CW", msg); // tell the simulator our CQ is over
                });

            IKnowDispatchIsCwSimulatorPtr pCwSidetone = pMgr->GetCwSidetoneGenerator(pNotify);
            pCwSidetone->PutWPM(25); // Lets do a typical contest CQ speed

            for (;;)
            {
                pRadio->MessageStartedNow();
                pCwSidetone->QueueToTransmitText(msg);
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
            pRadio->SetListenFrequency(7080, "RY");

             // see if we can get the simulator to answer us
            _bstr_t msg("CQ TEST DE W5XD");

            for (;;)
            {
                pRadio->MessageStartedNow();
                bool stop = ::MessageBox(0, "Simulator thinks you are sending RTTY now.\r\n OK to end the CQ (or cancel)", "SimClientTest", MB_YESNOCANCEL) != IDYES;
                if (stop)
                    break;
                pRadio->MessageCompletedNow("RY", msg);  // we call CQ in RTTY mode...the simulator may answer
                stop = ::MessageBox(0, "Again?", "SimClientTest", MB_YESNO) != IDYES;

            }

    }

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
}