
namespace Levitador_GMI_V2._0
{
    partial class Form1
    {
        /// <summary>
        /// Variable del diseñador necesaria.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Limpiar los recursos que se estén usando.
        /// </summary>
        /// <param name="disposing">true si los recursos administrados se deben desechar; false en caso contrario.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Código generado por el Diseñador de Windows Forms

        /// <summary>
        /// Método necesario para admitir el Diseñador. No se puede modificar
        /// el contenido de este método con el editor de código.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.btnVaciar = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.txtCoefDenZ0 = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.txtCoefDenZ2 = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.txtCoefDenZ1 = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.btnRestablecer = new System.Windows.Forms.Button();
            this.btnCancelar = new System.Windows.Forms.Button();
            this.chkReferencia = new System.Windows.Forms.CheckBox();
            this.label6 = new System.Windows.Forms.Label();
            this.chkCorriente = new System.Windows.Forms.CheckBox();
            this.chkPosicion = new System.Windows.Forms.CheckBox();
            this.chkCompensador = new System.Windows.Forms.CheckBox();
            this.txtGananciaInteg = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.txtCoefNumZ0 = new System.Windows.Forms.TextBox();
            this.txtCoefNumZ2 = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.txtCoefNumZ1 = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.btnIniciar = new System.Windows.Forms.Button();
            this.label10 = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.cmbPuerto = new System.Windows.Forms.ComboBox();
            this.label12 = new System.Windows.Forms.Label();
            this.btnConectar = new System.Windows.Forms.Button();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.lblStatus = new System.Windows.Forms.ToolStripStatusLabel();
            this.comPort = new System.IO.Ports.SerialPort(this.components);
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.txtIntervalo = new System.Windows.Forms.TextBox();
            this.statusStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnVaciar
            // 
            this.btnVaciar.Location = new System.Drawing.Point(671, 166);
            this.btnVaciar.Name = "btnVaciar";
            this.btnVaciar.Size = new System.Drawing.Size(75, 23);
            this.btnVaciar.TabIndex = 74;
            this.btnVaciar.Text = "Vaciar";
            this.btnVaciar.UseVisualStyleBackColor = true;
            this.btnVaciar.Click += new System.EventHandler(this.btnVaciar_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(366, 157);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(16, 13);
            this.label3.TabIndex = 72;
            this.label3.Text = " +";
            // 
            // txtCoefDenZ0
            // 
            this.txtCoefDenZ0.Location = new System.Drawing.Point(281, 189);
            this.txtCoefDenZ0.Name = "txtCoefDenZ0";
            this.txtCoefDenZ0.Size = new System.Drawing.Size(79, 20);
            this.txtCoefDenZ0.TabIndex = 71;
            this.txtCoefDenZ0.Text = "1";
            this.txtCoefDenZ0.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(366, 191);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(16, 13);
            this.label7.TabIndex = 70;
            this.label7.Text = " +";
            // 
            // txtCoefDenZ2
            // 
            this.txtCoefDenZ2.Location = new System.Drawing.Point(514, 188);
            this.txtCoefDenZ2.Name = "txtCoefDenZ2";
            this.txtCoefDenZ2.Size = new System.Drawing.Size(78, 20);
            this.txtCoefDenZ2.TabIndex = 69;
            this.txtCoefDenZ2.Text = "0.9573";
            this.txtCoefDenZ2.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(598, 192);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(27, 13);
            this.label8.TabIndex = 68;
            this.label8.Text = "z^-2";
            // 
            // txtCoefDenZ1
            // 
            this.txtCoefDenZ1.Location = new System.Drawing.Point(388, 189);
            this.txtCoefDenZ1.Name = "txtCoefDenZ1";
            this.txtCoefDenZ1.Size = new System.Drawing.Size(78, 20);
            this.txtCoefDenZ1.TabIndex = 67;
            this.txtCoefDenZ1.Text = "-1.947";
            this.txtCoefDenZ1.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(472, 192);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(36, 13);
            this.label9.TabIndex = 66;
            this.label9.Text = "z^-1 +";
            // 
            // btnRestablecer
            // 
            this.btnRestablecer.Location = new System.Drawing.Point(170, 383);
            this.btnRestablecer.Name = "btnRestablecer";
            this.btnRestablecer.Size = new System.Drawing.Size(117, 34);
            this.btnRestablecer.TabIndex = 65;
            this.btnRestablecer.Text = "Restablecer";
            this.btnRestablecer.UseVisualStyleBackColor = true;
            this.btnRestablecer.Click += new System.EventHandler(this.btnRestablecer_Click);
            // 
            // btnCancelar
            // 
            this.btnCancelar.Location = new System.Drawing.Point(474, 383);
            this.btnCancelar.Name = "btnCancelar";
            this.btnCancelar.Size = new System.Drawing.Size(117, 34);
            this.btnCancelar.TabIndex = 64;
            this.btnCancelar.Text = "Cancelar";
            this.btnCancelar.UseVisualStyleBackColor = true;
            this.btnCancelar.Click += new System.EventHandler(this.btnCancelar_Click);
            // 
            // chkReferencia
            // 
            this.chkReferencia.AutoSize = true;
            this.chkReferencia.Location = new System.Drawing.Point(280, 333);
            this.chkReferencia.Name = "chkReferencia";
            this.chkReferencia.Size = new System.Drawing.Size(135, 17);
            this.chkReferencia.TabIndex = 63;
            this.chkReferencia.Text = "Referencia de posición";
            this.chkReferencia.UseVisualStyleBackColor = true;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(108, 263);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(100, 13);
            this.label6.TabIndex = 62;
            this.label6.Text = "Variables a graficar:";
            // 
            // chkCorriente
            // 
            this.chkCorriente.AutoSize = true;
            this.chkCorriente.Location = new System.Drawing.Point(280, 309);
            this.chkCorriente.Name = "chkCorriente";
            this.chkCorriente.Size = new System.Drawing.Size(142, 17);
            this.chkCorriente.TabIndex = 61;
            this.chkCorriente.Text = "Corriente del electroimán";
            this.chkCorriente.UseVisualStyleBackColor = true;
            // 
            // chkPosicion
            // 
            this.chkPosicion.AutoSize = true;
            this.chkPosicion.Location = new System.Drawing.Point(280, 286);
            this.chkPosicion.Name = "chkPosicion";
            this.chkPosicion.Size = new System.Drawing.Size(111, 17);
            this.chkPosicion.TabIndex = 60;
            this.chkPosicion.Text = "Posición estimada";
            this.chkPosicion.UseVisualStyleBackColor = true;
            // 
            // chkCompensador
            // 
            this.chkCompensador.AutoSize = true;
            this.chkCompensador.Location = new System.Drawing.Point(280, 263);
            this.chkCompensador.Name = "chkCompensador";
            this.chkCompensador.Size = new System.Drawing.Size(139, 17);
            this.chkCompensador.TabIndex = 59;
            this.chkCompensador.Text = "Salida del compensador";
            this.chkCompensador.UseVisualStyleBackColor = true;
            // 
            // txtGananciaInteg
            // 
            this.txtGananciaInteg.Location = new System.Drawing.Point(280, 226);
            this.txtGananciaInteg.Name = "txtGananciaInteg";
            this.txtGananciaInteg.Size = new System.Drawing.Size(78, 20);
            this.txtGananciaInteg.TabIndex = 58;
            this.txtGananciaInteg.Text = "10";
            this.txtGananciaInteg.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(108, 226);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(123, 13);
            this.label5.TabIndex = 57;
            this.label5.Text = "Ganancia del integrador:";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(106, 171);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(68, 13);
            this.label4.TabIndex = 56;
            this.label4.Text = "Coeficientes:";
            // 
            // txtCoefNumZ0
            // 
            this.txtCoefNumZ0.Location = new System.Drawing.Point(281, 154);
            this.txtCoefNumZ0.Name = "txtCoefNumZ0";
            this.txtCoefNumZ0.Size = new System.Drawing.Size(79, 20);
            this.txtCoefNumZ0.TabIndex = 55;
            this.txtCoefNumZ0.Text = "2.422*10^6";
            this.txtCoefNumZ0.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // txtCoefNumZ2
            // 
            this.txtCoefNumZ2.Location = new System.Drawing.Point(514, 154);
            this.txtCoefNumZ2.Name = "txtCoefNumZ2";
            this.txtCoefNumZ2.Size = new System.Drawing.Size(78, 20);
            this.txtCoefNumZ2.TabIndex = 54;
            this.txtCoefNumZ2.Text = "2.416*10^6";
            this.txtCoefNumZ2.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(598, 157);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(30, 13);
            this.label2.TabIndex = 53;
            this.label2.Text = "z^-2 ";
            // 
            // txtCoefNumZ1
            // 
            this.txtCoefNumZ1.Location = new System.Drawing.Point(388, 154);
            this.txtCoefNumZ1.Name = "txtCoefNumZ1";
            this.txtCoefNumZ1.Size = new System.Drawing.Size(78, 20);
            this.txtCoefNumZ1.TabIndex = 52;
            this.txtCoefNumZ1.Text = "-4.838*10^6";
            this.txtCoefNumZ1.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(472, 157);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(36, 13);
            this.label1.TabIndex = 51;
            this.label1.Text = "z^-1 +";
            // 
            // btnIniciar
            // 
            this.btnIniciar.Enabled = false;
            this.btnIniciar.Location = new System.Drawing.Point(324, 383);
            this.btnIniciar.Name = "btnIniciar";
            this.btnIniciar.Size = new System.Drawing.Size(117, 34);
            this.btnIniciar.TabIndex = 50;
            this.btnIniciar.Text = "Iniciar";
            this.btnIniciar.UseVisualStyleBackColor = true;
            this.btnIniciar.Click += new System.EventHandler(this.btnIniciar_Click);
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(265, 173);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(364, 13);
            this.label10.TabIndex = 73;
            this.label10.Text = "---------------------------------------------------------------------------------" +
    "--------------------------------------";
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(271, 37);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(97, 13);
            this.label11.TabIndex = 76;
            this.label11.Text = "Seleccionar Puerto";
            // 
            // cmbPuerto
            // 
            this.cmbPuerto.FormattingEnabled = true;
            this.cmbPuerto.Location = new System.Drawing.Point(260, 67);
            this.cmbPuerto.Name = "cmbPuerto";
            this.cmbPuerto.Size = new System.Drawing.Size(121, 21);
            this.cmbPuerto.TabIndex = 75;
            this.cmbPuerto.SelectedIndexChanged += new System.EventHandler(this.cmbPuerto_SelectedIndexChanged_1);
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(-4, 122);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(808, 13);
            this.label12.TabIndex = 77;
            this.label12.Text = resources.GetString("label12.Text");
            // 
            // btnConectar
            // 
            this.btnConectar.Location = new System.Drawing.Point(463, 65);
            this.btnConectar.Name = "btnConectar";
            this.btnConectar.Size = new System.Drawing.Size(91, 23);
            this.btnConectar.TabIndex = 78;
            this.btnConectar.Text = "Conectar";
            this.btnConectar.UseVisualStyleBackColor = true;
            this.btnConectar.Click += new System.EventHandler(this.btnConectar_Click);
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.lblStatus});
            this.statusStrip1.Location = new System.Drawing.Point(0, 432);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(800, 22);
            this.statusStrip1.TabIndex = 79;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // lblStatus
            // 
            this.lblStatus.Name = "lblStatus";
            this.lblStatus.Size = new System.Drawing.Size(82, 17);
            this.lblStatus.Text = "Desconectado";
            // 
            // comPort
            // 
            this.comPort.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.comPort_DataReceived);
            // 
            // timer1
            // 
            this.timer1.Interval = 5000;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // txtIntervalo
            // 
            this.txtIntervalo.Location = new System.Drawing.Point(514, 330);
            this.txtIntervalo.Name = "txtIntervalo";
            this.txtIntervalo.Size = new System.Drawing.Size(100, 20);
            this.txtIntervalo.TabIndex = 80;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 454);
            this.Controls.Add(this.txtIntervalo);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.btnConectar);
            this.Controls.Add(this.label12);
            this.Controls.Add(this.label11);
            this.Controls.Add(this.cmbPuerto);
            this.Controls.Add(this.btnVaciar);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.txtCoefDenZ0);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.txtCoefDenZ2);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.txtCoefDenZ1);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.btnRestablecer);
            this.Controls.Add(this.btnCancelar);
            this.Controls.Add(this.chkReferencia);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.chkCorriente);
            this.Controls.Add(this.chkPosicion);
            this.Controls.Add(this.chkCompensador);
            this.Controls.Add(this.txtGananciaInteg);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.txtCoefNumZ0);
            this.Controls.Add(this.txtCoefNumZ2);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.txtCoefNumZ1);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.btnIniciar);
            this.Controls.Add(this.label10);
            this.Name = "Form1";
            this.Text = "Form1";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load_1);
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnVaciar;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox txtCoefDenZ0;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox txtCoefDenZ2;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TextBox txtCoefDenZ1;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Button btnRestablecer;
        private System.Windows.Forms.Button btnCancelar;
        private System.Windows.Forms.CheckBox chkReferencia;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.CheckBox chkCorriente;
        private System.Windows.Forms.CheckBox chkPosicion;
        private System.Windows.Forms.CheckBox chkCompensador;
        private System.Windows.Forms.TextBox txtGananciaInteg;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox txtCoefNumZ0;
        private System.Windows.Forms.TextBox txtCoefNumZ2;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox txtCoefNumZ1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btnIniciar;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.ComboBox cmbPuerto;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Button btnConectar;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel lblStatus;
        private System.IO.Ports.SerialPort comPort;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.TextBox txtIntervalo;
    }
}

