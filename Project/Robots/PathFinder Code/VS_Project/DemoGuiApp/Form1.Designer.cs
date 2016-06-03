namespace DemoGuiApp
{
    partial class Form1
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
            this.pathFinderControl1 = new PathFinderLibrary.PathFinderControl();
            this.SuspendLayout();
            // 
            // pathFinderControl1
            // 
            this.pathFinderControl1.BackColor = System.Drawing.SystemColors.ButtonShadow;
            this.pathFinderControl1.Board = null;
            this.pathFinderControl1.Location = new System.Drawing.Point(3, 3);
            this.pathFinderControl1.Name = "pathFinderControl1";
            this.pathFinderControl1.Size = new System.Drawing.Size(800, 600);
            this.pathFinderControl1.TabIndex = 5;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(804, 604);
            this.Controls.Add(this.pathFinderControl1);
            this.DoubleBuffered = true;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Pathfinder Demo Application";
            this.ResumeLayout(false);

        }

        #endregion

        private PathFinderLibrary.PathFinderControl pathFinderControl1;       

    }
}

