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
            this.textBoxCallUsed = new System.Windows.Forms.TextBox();
            this.comboBoxContestSelection = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.buttonUpdate = new System.Windows.Forms.Button();
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
            // textBoxCallUsed
            // 
            this.textBoxCallUsed.Location = new System.Drawing.Point(134, 111);
            this.textBoxCallUsed.Name = "textBoxCallUsed";
            this.textBoxCallUsed.Size = new System.Drawing.Size(66, 20);
            this.textBoxCallUsed.TabIndex = 4;
            // 
            // comboBoxContestSelection
            // 
            this.comboBoxContestSelection.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxContestSelection.FormattingEnabled = true;
            this.comboBoxContestSelection.Items.AddRange(new object[] {
            "Simple",
            "WPX",
            "ARRL SS",
            "IARU HF"});
            this.comboBoxContestSelection.Location = new System.Drawing.Point(134, 149);
            this.comboBoxContestSelection.Name = "comboBoxContestSelection";
            this.comboBoxContestSelection.Size = new System.Drawing.Size(66, 21);
            this.comboBoxContestSelection.TabIndex = 5;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(74, 114);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(53, 13);
            this.label1.TabIndex = 6;
            this.label1.Text = "Call used:";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(90, 152);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(37, 13);
            this.label2.TabIndex = 7;
            this.label2.Text = "Rules:";
            this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // buttonUpdate
            // 
            this.buttonUpdate.Location = new System.Drawing.Point(225, 129);
            this.buttonUpdate.Name = "buttonUpdate";
            this.buttonUpdate.Size = new System.Drawing.Size(75, 23);
            this.buttonUpdate.TabIndex = 8;
            this.buttonUpdate.Text = "Update";
            this.buttonUpdate.UseVisualStyleBackColor = true;
            this.buttonUpdate.Click += new System.EventHandler(this.buttonUpdate_Click);
            // 
            // CssClientForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(319, 187);
            this.Controls.Add(this.buttonUpdate);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.comboBoxContestSelection);
            this.Controls.Add(this.textBoxCallUsed);
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
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonDemoSidetone;
        private System.Windows.Forms.Button buttonCwQso;
        private System.Windows.Forms.Button buttonDemoRy;
        private System.Windows.Forms.Button buttonDemoBandmap;
        private System.Windows.Forms.TextBox textBoxCallUsed;
        private System.Windows.Forms.ComboBox comboBoxContestSelection;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button buttonUpdate;
    }
}

