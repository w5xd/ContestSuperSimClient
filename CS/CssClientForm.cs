using System;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace CSSclient
{

    public partial class CssClientForm : Form
    {
        public CssClientForm()
        {
            InitializeComponent();
        }

        CSS cssManager;

        private void CssClientForm_Load(object sender, EventArgs e)
        {
            try
            {
                cssManager = new CSS();
            }
            catch (System.Exception exc)
            {
                MessageBox.Show("Failed to connect to Contest Super Simulator");
                throw exc;
            }
        }


        [ComVisible(true)]
        public class NotifyNoop
        {
            [ComVisible(true), DispId(1)]
            public void CwQueued(int MessageId)
            {
#if DEBUG
                System.Diagnostics.Debug.WriteLine(String.Format("CwQueued {0}", MessageId));
#endif
            }
            [ComVisible(true), DispId(2)]
            public void CwFinished(int MessageId, string TextSent)
            {
#if DEBUG
                System.Diagnostics.Debug.WriteLine(String.Format("CwFinished {0} \"{1}\"", MessageId, TextSent));
#endif
            }
        }

        private void buttonDemoSidetone_Click(object sender, EventArgs e)
        {
            // Nothing here but sidetone generation
            // Client of simulator can call its winkey emulation or call on
            // its ICwSimulator, but its redundant to do both.
            var CwSidetone = cssManager.GetCwSidetoneGenerator(new NotifyNoop());
            CwSidetone.WPM = 25;
            CwSidetone.QueueToTransmitText("test de w5xd");
            CwSidetone.QueueWpmChange(35);
            CwSidetone.QueueToTransmitText("test test w5xd");
        }

        [ComVisible(true)]
        public class NotifyHelper
        {
            public delegate void DoneFcn();
            private DoneFcn m_doneFcn;
            public NotifyHelper(DoneFcn doneFcn)
            {
                m_doneFcn = doneFcn;
            }
            private int m_lastMessageStarted = 0;
            [ComVisible(true), DispId(1)]
            public void CwQueued(int MessageId)
            {
                m_lastMessageStarted = MessageId;
            }
            [ComVisible(true), DispId(2)]
            public void CwFinished(int MessageId, string TextSent)
            {
                // this callback happens when the CQ finishes..
                // Will ALSO get call backs for WinKey-initiated message finished...
                // Client should choose one or the other           }
                if (MessageId >= m_lastMessageStarted && null != m_doneFcn)
                    m_doneFcn();
            }
        }

        private void buttonCwQso_Click(object sender, EventArgs e)
        {
            Radio radio = cssManager.GetSimulatorForRadio(0); 
            string message = "CQ TEST DE W5XD";

            // Demonstrate the timing sequence.
            NotifyHelper notify = new NotifyHelper(() => {
                radio.MessageCompletedNow("CW", message);
                });

            CwSideToneSimulator cwSidetone = cssManager.GetCwSidetoneGenerator(notify);
            cwSidetone.WPM = 25; // Lets do a typical contest CQ speed
            short sidetoneMask = 3;
            for (; ; )
            {
                radio.MessageStartedNow();
                cwSidetone.QueueToTransmitText(message);
                cwSidetone.HeadphoneMask = sidetoneMask;
                sidetoneMask = (short)((int)sidetoneMask % 3 + 1);
                var yesno = MessageBox.Show(this, "CQ again? No to Exit", "CSS Client Test", MessageBoxButtons.YesNo);
                if (yesno == DialogResult.No)
                    break;
            }

        }

        private void buttonDemoRy_Click(object sender, EventArgs e)
        {
            Radio radio = cssManager.GetSimulatorForRadio(0);
            // tune the virtual radio to RTTY
            // This demo would work on the simulator's default frequency, but
            // this one is a more conventional RTTY frequency.
            // SetListenFrequency is the key for "tuning" the simulator through its
            // virtual bands on CW as well as RTTY.
            radio.SetListenFrequency(7080.0, "RY");
            string msg = "CQ TEST DE W5XD";

            /* the drill of calling MessageStartedNow and MessageCompletedNow is shown for RTTY,
            ** but can be used on CW as an alternative to WinKey*/
            for (; ; )
            {
                radio.MessageStartedNow();
                bool stop = MessageBox.Show(this,
                    "Simulator thinks you are sending RTTY now.\r\n OK to end the CQ (or cancel)",
                    "SimClientTest", MessageBoxButtons.YesNoCancel) != DialogResult.Yes;
                if (stop)
                    break;
                radio.MessageCompletedNow("RY", msg);  // we call CQ in RTTY mode...the simulator may answer
                stop = MessageBox.Show(this, "Again?", "SimClientTest", MessageBoxButtons.YesNo) != DialogResult.Yes;
            }

        }

        private void buttonDemoBandmap_Click(object sender, EventArgs e)
        {
            /* Client might, for example, populate a Band Map with the info it gets here    */
            short radioNumber = 0;
            // the simulator will launch some CPU-intensive tasks on GetSimulatorForRadio.
            // So only ask it for radios that you want to pay the price for simulation.
            // The simulator does not offer a way to shut down a simulated radio once started.
            Radio radio = cssManager.GetSimulatorForRadio(radioNumber);
            StationList stations = radio.SimulatedStations;
            short count = (short)stations.Count;
            for (short i = 0; i < count; i++)
            {
#if DEBUG
                System.Diagnostics.Debug.WriteLine(String.Format("Contestant {0} {1} CQ:{2} Freq:{3}", 
                    stations.GetCallsign(i), stations.GetModulationMode(i), stations.GetSpOrCq(i), stations.GetFreqKhz(i) ));
#endif
            }
        }
    }
}
