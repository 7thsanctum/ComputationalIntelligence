using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using PathFinderLibrary;
using System.Threading;

namespace DemoGuiApp
{
    public partial class Form1 : Form
    {
        
        public Form1()
        {
            InitializeComponent();
            this.pathFinderControl1.SubmitClicked +=
                new PathFinderLibrary.PathFinderControl.SubmitClickedHandler(pathFinderCtrl_SubmitClicked);
        }
        
        void pathFinderCtrl_SubmitClicked(Board board)
        {
            AbstractAlgorithm alg = new DemoGuiApplication(board);
            pathFinderControl1.start(alg);
        }        
        

       
    }
}
