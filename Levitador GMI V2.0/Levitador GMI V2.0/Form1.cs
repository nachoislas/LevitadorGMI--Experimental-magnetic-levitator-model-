using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;
using System.IO.Ports;

namespace Levitador_GMI_V2._0
{
    public partial class Form1 : Form
    {
         string strPuerto;       //string para guardar el nombre del puerto serie
                                 //lo hago acá para que sea de alcance 
                                 //global dentro de esta clase

        string[] datos;     //string array que guarda los datos recibidos por puerto serie separados por ,
        bool newDataParaGraficar = false;
        bool levitadorConectado = false; //indica si el micro está conectado o no
        

        string datoComp, datoCorr, datoPos, datoRef;
      

        //coeficientes por defecto
        const string COEF_NUM_Z0 = "2.422e6";
        const string COEF_NUM_Z1 = "-4.838e6";
        const string COEF_NUM_Z2 = "2.416e6";
        const string COEF_DEN_Z0 = "1";
        const string COEF_DEN_Z1 = "-1.947";
        const string COEF_DEN_Z2 = "0.953";

        public Form1()
        {
            //Creamos hilo
            Thread t = new Thread(new ThreadStart(SplashStart));
            //Arrancamos el hilo
            t.Start();

            //Ponemos a dormir la forma principal. Aca, en vez de Thread.Sleep(), iria todo el 
            //codigo necesario para cargar lo que necesitamos en nuestra aplicacion mientras se
            //este mostrando el splash.Pero las asignaciones para los controles propios de la forma

            Thread.Sleep(2500);

            InitializeComponent();

            //Finalizamos el hilo
            t.Abort();
            this.BringToFront();
        }
        public void SplashStart()
        {
            Application.Run(new Splash());
        }

       

        private void cmbPuerto_SelectedIndexChanged(object sender, EventArgs e)
        {
            //cada vez que se cambie la seleccion en el combo box, se actaliza el strPuerto.
            int indice = cmbPuerto.SelectedIndex;
            strPuerto = cmbPuerto.Items[indice].ToString();
        }

        private void Form1_Load_1(object sender, EventArgs e)
        {
            //cargamos los nombres de puertos en el combo box
            string[] ports = SerialPort.GetPortNames();
            foreach (string port in ports)
            {
                cmbPuerto.Items.Add(port);
            }

            // cargamos los valores predefinidos en la función transferencia
            setDefaultCoef();
        }

        private void setDefaultCoef()
        {
            txtCoefDenZ0.Text = COEF_DEN_Z0;
            txtCoefDenZ1.Text = COEF_DEN_Z1;
            txtCoefDenZ2.Text = COEF_DEN_Z2;
            txtCoefNumZ0.Text = COEF_NUM_Z0;
            txtCoefNumZ1.Text = COEF_NUM_Z1;
            txtCoefNumZ2.Text = COEF_NUM_Z2;
        }

        private void btnConectar_Click(object sender, EventArgs e)
        {
            if (!levitadorConectado)
            {
                if (strPuerto != "")
                    comPort.PortName = strPuerto;   //se le asigna strPuerto a la propiedad PortName de comPort.

                comPort.BaudRate = 9600;

                try
                {
                    comPort.Open();      //abrimos el puerto
                    timer1.Enabled = true;
                    btnIniciar.Enabled = true;
                    levitadorConectado = true;
                }
                catch (Exception)
                {
                    //Console.Write("No se pudo conectar");
                }
                btnConectar.Text = "Desconectar";
            }
            else
            {
                btnConectar.Text = "Conectar";
                comPort.Close();
                levitadorConectado = false;
                btnIniciar.Enabled = false;
            }
           
        }

        private void btnVaciar_Click(object sender, EventArgs e)
        {
            txtCoefNumZ0.Text = "";
            txtCoefNumZ1.Text = "";
            txtCoefNumZ2.Text = "";
            txtCoefDenZ0.Text = "";
            txtCoefDenZ1.Text = "";
            txtCoefDenZ2.Text = "";
        }

        private void btnRestablecer_Click(object sender, EventArgs e)
        {
            setDefaultCoef();

            txtGananciaInteg.Text = "10";
        }

        private void btnIniciar_Click(object sender, EventArgs e)
        {
            //creo un objeto para cada gráfico, todos de la misma clase Graficos
            //lo hago acá para que se cree un objeto nuevo cada vez que se aprieta el botón iniciar
            Graficos VentanaCorriente = new Graficos("Controlador de corriente");
            Graficos VentanaCompensador = new Graficos("Salida del compensador");
            Graficos VentanaPosicion = new Graficos("Posición estimada");
            Graficos VentanaReferencia = new Graficos("Referencia de posición");

            if (chkCompensador.Checked)
            {
                VentanaCompensador.Show();
            }
            if (chkCorriente.Checked)
            {
                VentanaCorriente.Show();
            }
            if (chkPosicion.Checked)
            {
                VentanaPosicion.Show();
            }
            if (chkReferencia.Checked)
            {
                VentanaReferencia.Show();
            }

            Thread ventanasThread = new Thread(() => actualizarGraficos(VentanaCompensador,
                                                                            VentanaCorriente,
                                                                            VentanaPosicion,
                                                                            VentanaReferencia));

            ventanasThread.IsBackground = true;
            ventanasThread.Start();

            string CoefNumZ0 = txtCoefNumZ0.Text;
            string CoefNumZ1 = txtCoefNumZ1.Text;
            string CoefNumZ2 = txtCoefNumZ2.Text;
            string CoefDenZ0 = txtCoefDenZ0.Text;
            string CoefDenZ1 = txtCoefDenZ1.Text;
            string CoefDenZ2 = txtCoefDenZ2.Text;

            string GananciaInteg = txtGananciaInteg.Text;

            string mensajeInicio = "Inicio," + CoefNumZ0 + "," + CoefNumZ1 + "," + CoefNumZ2 + "," + CoefDenZ0 + "," + CoefDenZ1 + "," + CoefDenZ2 + "," + GananciaInteg + "\r\n";

            if(levitadorConectado)      comPort.Write(mensajeInicio);


        }

        private void actualizarGraficos(Graficos comp, Graficos corr, Graficos pos, Graficos referencia)
        {
            while (true)
            {
                if (newDataParaGraficar)
                {
                    newDataParaGraficar = false;
                    comp.ValorSerie = datoComp;
                    corr.ValorSerie = datoCorr;
                    pos.ValorSerie = datoPos;
                    referencia.ValorSerie = datoRef;
                }
                //cierro el thread si se cierran los graficos
                if(comp.IsDisposed | corr.IsDisposed | pos.IsDisposed | referencia.IsDisposed)
                {
                    comp.Close();
                    corr.Close();
                    pos.Close();
                    referencia.Close();
                    break;
                }
            }
        }

        private void btnCancelar_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        
        private void comPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            string DatoSerialRecibido = comPort.ReadLine();         //leemos el dato recibido hasta encontrar \n
            //acá deberíamos hacer una decodificacion del comando recibido, y en base a eso mandarlos a los graficos, o lo que se
            datos = DatoSerialRecibido.Split(',');
            //txtRcv.Text = datos[4];
            Comm_parse(datos);
        }

       
        private void Comm_parse(string[] datos)
        {
            string comando = datos[0];
            switch (comando)
            {
                case "Datos":
                    datoCorr = datos[1];
                    datoComp = datos[2];
                    datoPos = datos[3];
                    datoRef = datos[4];
                    newDataParaGraficar = true;
                    break;

                case "CONECTADO\r":
                    lblStatus.Text = "Conectado";
                    timer1.Stop();
                    timer1.Start();
                    break;

                default:
                   // txtRcv.Text = "Error";
                    break;
            }

            
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            timer1.Stop();
            lblStatus.Text = "Desconectado";
            levitadorConectado = false;
            btnIniciar.Enabled = false;
            MessageBox.Show("Dispositivo desconectado");
            
            
        }

        private void cmbPuerto_SelectedIndexChanged_1(object sender, EventArgs e)
        {
            //cada vez que se cambie la seleccion en el combo box, se actaliza el strPuerto.
            int indice = cmbPuerto.SelectedIndex;
            strPuerto = cmbPuerto.Items[indice].ToString();
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            comPort.Close();
        }
    }
}
