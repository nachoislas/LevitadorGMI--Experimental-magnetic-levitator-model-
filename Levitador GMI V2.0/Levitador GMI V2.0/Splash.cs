﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;


namespace Levitador_GMI_V2._0
{
    public partial class Splash : Form
    {
        public Splash()
        {
            InitializeComponent();
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            //Incrementamos la barra
            progressBar1.Increment(3);
            //Si llega al máximo paramos el timer
            if (progressBar1.Value == 100)
                timer1.Stop();
        }
    }
}
