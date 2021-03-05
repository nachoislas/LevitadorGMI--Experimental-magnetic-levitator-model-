using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;

namespace Levitador_GMI_V2._0
{
    public partial class graficos_v2 : Form
    {
        //objeto para poder acceder al form principal desde acá
        private Form1 parentForm;

        private string[] datos = new string[5];
        private string datoComp, datoCorr, datoPos, datoRef;

        private double X;
        


        public graficos_v2(Form1 parent)
        {
            InitializeComponent();
            this.parentForm = parent;
        }

        private void graficos_v2_Load(object sender, EventArgs e)
        {
            
            timerPlot.Interval = 30;
            timerPlot.Start();
        }

        private void graficos_v2_FormClosing(object sender, FormClosingEventArgs e)
        {
            parentForm.SerialPort_write("DETENER\r\n");
        }

        private int muestra = 0;
        private void timerPlot_Tick(object sender, EventArgs e)
        {
            //actualizar gráfico
            if (parentForm.DatosNuevos)
            {
                parentForm.DatosNuevos = false;
                //datos = parentForm.Datos;
                try
                {
                    datoCorr = parentForm.Datos(1);
                    datoComp = parentForm.Datos(2);
                    datoPos = parentForm.Datos(3);
                    datoRef = parentForm.Datos(4);
                }
                catch
                {
                    //
                }

                chart_update(chartCorriente, datoCorr, 0);
                chart_update(chartCompensador, datoComp, 0);
                chart_update(chartPosicion, datoPos, 0);
                chart_update(chartPosicion, datoRef, 1);
                muestra++;
                X += 0.1;
                X = Math.Round(X, 1);

            }
        }

        private void chart_update(Chart chart, string valor, int serie)
        {
            if (valor != "")
            {
                float Y = float.Parse(valor);       //convierte lo recibido a float


                //asigna el valor recibido para que sea graficado
                chart.Invoke((MethodInvoker)(() =>
                         chart.Series[serie].Points.AddXY(X, Y)));
                //   chart.Series[serie].Points.AddXY(X, Y);

                if (chart.Series[serie].Points.Count > 15)
                    chart.Series[serie].Points.RemoveAt(0);

                chart.ChartAreas[0].AxisX.Minimum = chart.Series[serie].Points[0].XValue;
                chart.ChartAreas[0].AxisX.Maximum = X;
                chart.ChartAreas[0].AxisY.Maximum = 4096;
            }
        }
    }
}
