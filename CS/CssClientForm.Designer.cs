namespace CSSclient
{
    partial class CssClientForm
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
            this.buttonDemoSidetone = new System.Windows.Forms.Button();
            this.buttonCwQso = new System.Windows.Forms.Button();
            this.buttonDemoRy = new System.Windows.Forms.Button();
            this.buttonDemoBandmap = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // buttonDemoSidetone
            // 
            this.buttonDemoSidetone.Location = new System.Drawing.Point(12, 21);
            this.buttonDemoSidetone.Name = "buttonDemoSidetone";
            this.buttonDemoSidetone.Size = new System.Drawing.Size(116, 23);
            this.buttonDemoSidetone.TabIndex = 0;
            this.buttonDemoSidetone.Text = "Demo sidetone";
            this.buttonDemoSidetone.UseVisualStyleBackColor = true;
            this.buttonDemoSidetone.Click += new System.EventHandler(this.buttonDemoSidetone_Click);
            // 
            // buttonCwQso
            // 
            this.buttonCwQso.Location = new System.Drawing.Point(134, 21);
            this.buttonCwQso.Name = "buttonCwQso";
            this.buttonCwQso.Size = new System.Drawing.Size(75, 23);
            this.buttonCwQso.TabIndex = 1;
            this.buttonCwQso.Text = "Demo CW QSO";
            this.buttonCwQso.UseVisualStyleBackColor = true;
            this.buttonCwQso.Click += new System.EventHandler(this.buttonCwQso_Click);
            // 
            // buttonDemoRy
            // 
            this.buttonDemoRy.Location = new System.Drawing.Point(225, 21);
            this.buttonDemoRy.Name = "buttonDemoRy";
            this.buttonDemoRy.Size = new System.Drawing.Size(75, 23);
            this.buttonDemoRy.TabIndex = 2;
            this.buttonDemoRy.Text = "Demo RTTY";
            this.buttonDemoRy.UseVisualStyleBackColor = true;
            this.buttonDemoRy.Click += new System.EventHandler(this.buttonDemoRy_Click);
            // 
            // buttonDemoBandmap
            // 
            this.buttonDemoBandmap.Location = new System.Drawing.Point(12, 62);
            this.buttonDemoBandmap.Name = "buttonDemoBandmap";
            this.buttonDemoBandmap.Size = new System.Drawing.Size(116, 23);
            this.buttonDemoBandmap.TabIndex = 3;
            this.buttonDemoBandmap.Text = "Demo Band Map list";
            this.buttonDemoBandmap.UseVisualStyleBackColor = true;
            this.buttonDemoBandmap.Click += new System.EventHandler(this.buttonDemoBandmap_Click);
            // 
            // CssClientForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(319, 103);
            this.Controls.Add(this.buttonDemoBandmap);
            this.Controls.Add(this.buttonDemoRy);
            this.Controls.Add(this.buttonCwQso);
            this.Controls.Add(this.buttonDemoSidetone);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "CssClientForm";
            this.Text = "Contest Super Simulator Client Demo";
            this.Load += new System.EventHandler(this.CssClientForm_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button buttonDemoSidetone;
        private System.Windows.Forms.Button buttonCwQso;
        private System.Windows.Forms.Button buttonDemoRy;
        private System.Windows.Forms.Button buttonDemoBandmap;
    }
}

