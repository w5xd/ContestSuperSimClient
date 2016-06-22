// SimClientTest.cpp : Defines the entry point for the console application.
//
#include <string>
#include <atlbase.h>
#include <iostream>

#include "SimIntefaceHelper.h"
#include "CwSidetoneNotifyImpl.h"

static void DoWithCOM();

int main(int argc, char* argv[])
{
    ::CoInitialize(0);
    DoWithCOM();    // COM objects must be released BEFORE CoUninitialize
    ::CoUninitialize();
	return 0;
}

static void DoWithCOM()
{
    try
    {
        ::MessageBox(0, "OK to continue", "SimClientTest", MB_OK);

        // The simulator manager registers itself in the ROT when the user runs it.
        IKnowDispatchIsManagerPtr        pAuto;
        pAuto.GetActiveObject(__uuidof(ContestSuperSimDispLib::SuperSimManager));

        if (!pAuto)
        {
            std::cout << "SuperSimManager not active" << std::endl;
            return;
        }

        // The manager object provides access to the virtual radio objects.
        // It supports radios numbered zero through 3

        IKnowDispatchIsRadioSimulatorPtr pRadio =
            pAuto->GetSimulatorForRadio(0);

        std::cout << "Got radio of frequency: " << pRadio->CenterFrequency << " KHz" << std::endl;

        MyNotifier *pNotify = new MyNotifier();

        IKnowDispatchIsCwSimulatorPtr pCwSidetone =
            pAuto->GetCwSidetoneGenerator(pNotify);

        if (!pCwSidetone)
        {
            std::cout << "GetCwSidetoneGenerate returned empty!" << std::endl;
            return;
        }

        pCwSidetone->PutWPM(25);
        pCwSidetone->QueueToTransmitText("test de w5xd");
        pCwSidetone->QueueWpmChange(35);
        pCwSidetone->QueueToTransmitText("test test w5xd");
        ::MessageBox(0, "OK to exit", "SimClientTest", MB_OK);
        pCwSidetone->AbortTransmission();
        pNotify->Release();
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
