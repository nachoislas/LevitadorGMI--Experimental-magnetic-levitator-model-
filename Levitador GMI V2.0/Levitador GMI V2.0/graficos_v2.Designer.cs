
namespace Levitador_GMI_V2._0
{
    partial class graficos_v2
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
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea4 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend4 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series5 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea5 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend5 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series6 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea6 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend6 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series7 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.Series series8 = new System.Windows.Forms.DataVisualization.Charting.Series();
            this.chartCorriente = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.timerPlot = new System.Windows.Forms.Timer(this.components);
            this.chartCompensador = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.chartPosicion = new System.Windows.Forms.DataVisualization.Charting.Chart();
            ((System.ComponentModel.ISupportInitialize)(this.chartCorriente)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.chartCompensador)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.chartPosicion)).BeginInit();
            this.SuspendLayout();
            // 
            // chartCorriente
            // 
            chartArea4.Name = "ChartArea1";
            this.chartCorriente.ChartAreas.Add(chartArea4);
            legend4.Enabled = false;
            legend4.Name = "Legend1";
            this.chartCorriente.Legends.Add(legend4);
            this.chartCorriente.Location = new System.Drawing.Point(12, 12);
            this.chartCorriente.Name = "chartCorriente";
            series5.ChartArea = "ChartArea1";
            series5.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            series5.Legend = "Legend1";
            series5.Name = "series1";
            this.chartCorriente.Series.Add(series5);
            this.chartCorriente.Size = new System.Drawing.Size(962, 181);
            this.chartCorriente.TabIndex = 0;
            this.chartCorriente.Text = "chart1";
            // 
            // timerPlot
            // 
            this.timerPlot.Tick += new System.EventHandler(this.timerPlot_Tick);
            // 
            // chartCompensador
            // 
            chartArea5.Name = "ChartArea1";
            this.chartCompensador.ChartAreas.Add(chartArea5);
            legend5.Enabled = false;
            legend5.Name = "Legend1";
            this.chartCompensador.Legends.Add(legend5);
            this.chartCompensador.Location = new System.Drawing.Point(12, 211);
            this.chartCompensador.Name = "chartCompensador";
            series6.ChartArea = "ChartArea1";
            series6.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            series6.Legend = "Legend1";
            series6.Name = "series1";
            this.chartCompensador.Series.Add(series6);
            this.chartCompensador.Size = new System.Drawing.Size(962, 181);
            this.chartCompensador.TabIndex = 1;
            this.chartCompensador.Text = "chart1";
            // 
            // chartPosicion
            // 
            chartArea6.Name = "ChartArea1";
            this.chartPosicion.ChartAreas.Add(chartArea6);
            legend6.Enabled = false;
            legend6.Name = "Legend1";
            this.chartPosicion.Legends.Add(legend6);
            this.chartPosicion.Location = new System.Drawing.Point(12, 420);
            this.chartPosicion.Name = "chartPosicion";
            series7.ChartArea = "ChartArea1";
            series7.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            series7.Legend = "Legend1";
            series7.Name = "series1";
            series8.ChartArea = "ChartArea1";
            series8.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            series8.Legend = "Legend1";
            series8.Name = "Series2";
            this.chartPosicion.Series.Add(series7);
            this.chartPosicion.Series.Add(series8);
            this.chartPosicion.Size = new System.Drawing.Size(962, 181);
            this.chartPosicion.TabIndex = 2;
            this.chartPosicion.Text = "chart1";
            // 
            // graficos_v2
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1004, 749);
            this.Controls.Add(this.chartPosicion);
            this.Controls.Add(this.chartCompensador);
            this.Controls.Add(this.chartCorriente);
            this.Name = "graficos_v2";
            this.Text = "graficos_v2";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.graficos_v2_FormClosing);
            this.Load += new System.EventHandler(this.graficos_v2_Load);
            ((System.ComponentModel.ISupportInitialize)(this.chartCorriente)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.chartCompensador)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.chartPosicion)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.DataVisualization.Charting.Chart chartCorriente;
        private System.Windows.Forms.Timer timerPlot;
        private System.Windows.Forms.DataVisualization.Charting.Chart chartCompensador;
        private System.Windows.Forms.DataVisualization.Charting.Chart chartPosicion;
    }
}