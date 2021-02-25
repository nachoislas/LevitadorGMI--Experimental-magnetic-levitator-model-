
namespace Levitador_GMI_V2._0
{
    partial class Graficos
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea1 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend1 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            this.grafCorriente = new System.Windows.Forms.DataVisualization.Charting.Chart();
            ((System.ComponentModel.ISupportInitialize)(this.grafCorriente)).BeginInit();
            this.SuspendLayout();
            // 
            // grafCorriente
            // 
            chartArea1.Name = "ChartArea1";
            this.grafCorriente.ChartAreas.Add(chartArea1);
            legend1.Name = "Legend1";
            this.grafCorriente.Legends.Add(legend1);
            this.grafCorriente.Location = new System.Drawing.Point(-1, 1);
            this.grafCorriente.Name = "grafCorriente";
            this.grafCorriente.Size = new System.Drawing.Size(802, 449);
            this.grafCorriente.TabIndex = 1;
            this.grafCorriente.Text = "Controlador de corriente";
            // 
            // Graficos
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.grafCorriente);
            this.Name = "Graficos";
            this.Text = "Graficos";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Graficos_FormClosing);
            this.Load += new System.EventHandler(this.Graficos_Load);
            ((System.ComponentModel.ISupportInitialize)(this.grafCorriente)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.DataVisualization.Charting.Chart grafCorriente;
    }
}