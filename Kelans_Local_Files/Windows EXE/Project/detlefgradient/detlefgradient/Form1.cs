using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace detlefgradient
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            int parsedValue;
            if ((String.IsNullOrEmpty(textBox1.Text)) || (String.IsNullOrEmpty(textBox2.Text)) || (String.IsNullOrEmpty(textBox3.Text)) || (String.IsNullOrEmpty(textBox4.Text)))
            {
                MessageBox.Show("Insert Values!!");
            }

            else if ((!int.TryParse(textBox1.Text, out parsedValue)) || (!int.TryParse(textBox2.Text, out parsedValue)) || (!int.TryParse(textBox3.Text, out parsedValue)) || (!int.TryParse(textBox4.Text, out parsedValue)))
            {
                MessageBox.Show("Letters are not a valid input!");
                return;
            }
            else {
                int time1 = int.Parse(textBox1.Text);
                int percentageA = int.Parse(textBox2.Text);
                int time2 = int.Parse(textBox3.Text);
                int percentageB = int.Parse(textBox4.Text);
                if ((time1 > 0) && (time1 <= 10) && (time2 > time1) && (time2 <= 360 && (percentageA >= 0) && (percentageA < percentageB) && (percentageB <= 100)))
                {
                    int ITERATIONS = 10000;
                    float[] times = new float[ITERATIONS];
                    int Bandwidth = time2 - time1;


                    for (int NN = 1; NN <= ITERATIONS; NN++)
                    {
                        float total = 0;
                        for (int i = 0; i <= NN; i++)
                        {
                            float Ai = 0;
                            Ai = (float)percentageA + ((float)(percentageB - percentageA) / NN) * i;
                            total += (float)Math.Floor((float)500 / Ai * 1000) / 1000;
                        }
                        times[NN - 1] = total;
                    }

                    float[] difference = new float[ITERATIONS];
                    for (int x = 0; x < ITERATIONS; x++)
                    {
                        float helper = 0;
                        helper = times[x] - (Bandwidth * 60);
                        if (helper < 0)
                        {
                            helper *= -1;
                        }
                        difference[x] = helper;
                    }

                    int minindex = 0;
                    float min = difference[minindex];
                    for (int x = 1; x < ITERATIONS; x++)
                    {
                        if (difference[x] < min)
                        {
                            min = difference[x];
                            minindex = x;
                        }
                    }

                    float expecteddifference = difference[minindex];
                    int No = minindex + 1;
                    float deltapercentage = (float)(percentageB - percentageA) / (No);
                    MessageBox.Show(String.Format("N = {0}", No));
                    MessageBox.Show(String.Format("The Linear Gradient will take {0} seconds more than Time 2", expecteddifference));
                    MessageBox.Show(String.Format("The Linear Gradient will increase {0}% each step", deltapercentage));
                }
                else {
                    MessageBox.Show("Incorrect Input!!");
                }
            }
            
        }
    }
}
