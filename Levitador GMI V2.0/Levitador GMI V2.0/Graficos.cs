using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Levitador_GMI_V2._0
{
    public partial class Graficos : Form
    {

        private string valorserie;      //variable para guardar los datos recibidos para cada gráfico
        int muestra = 0;
        bool newData = false;           //para saber si hay nuevos datos recibidos  
        string series;                  //para guardar el nombre de la serie de datos
        bool stopThread = false;        //para indicarle al thread si debe detenerse al cerrar la ventana de gráfico

        public Graficos(string title)   //el constructor recibe un parámetro para darle nombre al título y a la serie de datos
        {
            InitializeComponent();
            this.Text = title;
            series = title;
            grafCorriente.Series.Add(series);
            //configuramos el tipo de gráfico para que sea line (me parece mejor que spline)
            grafCorriente.Series[series].ChartType =
                            System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;

            //falta agregar funciones para cambiar el color y darle personalizacion
        }
    

        public string ValorSerie    //esto se usa para asignarle un valora valorserie, y además activar newData
        {
            set
            {
                valorserie = value;
                newData = true;
            }
        }

        private void graficar() //funcion que se encarga del graficado

        {
            while (true)        //es un ciclo infinito
            {
                try
                {
                    if (newData)
                    {
                        newData = false;
                        // decimal aux = Convert.ToDecimal(valorserie);

                        //  decimal aux2 = aux;
                        float aux2 = float.Parse(valorserie);       //convierte lo recibido a float
                        //asigna el valor recibido para que sea graficado
                        grafCorriente.Invoke((MethodInvoker)(() => grafCorriente.Series[series].Points.AddXY(muestra, aux2)));
                        muestra++;
                    }
                    if (stopThread)
                        break;      //sale del while y se finaliza el thread
                }

                catch (Exception)
                {

                }
            }
        }

        

        private void btnCerrar_Click(object sender, EventArgs e)
        {
            //grafCorriente.Series[series].Points.Clear();
            stopThread = true;  //detiene el thread al cerrarse la ventana
            this.Hide();
        }

        private void Graficos_Load(object sender, EventArgs e)
        {
            Thread graphThread = new Thread(graficar);  //creamos un thread para que se encargue del graficado
            graphThread.IsBackground = true;
            graphThread.Start();
        }

        private void Graficos_FormClosing(object sender, FormClosingEventArgs e)
        {
            stopThread = true;  //detiene el thread al cerrarse la ventana
           /* if (e.CloseReason == CloseReason.UserClosing)
            {
                e.Cancel = true;
                this.Hide();
            }*/
        }
    }
}
