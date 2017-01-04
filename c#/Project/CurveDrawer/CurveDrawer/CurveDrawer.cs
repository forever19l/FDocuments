using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.Runtime.InteropServices;
using System.IO.Compression;
using System.Text.RegularExpressions;

namespace CurveDrawer
{

    public partial class CurveDrawer : Form
    {
        public static int MARK_POINT_NUM = 15;
        public static int POINT_X_LENGTH = 4;
        public static int POINT_Y_LENGTH = 4;
        public static int MAX_RAW_DATALENGTH = 1024 * 10 * sizeof(float);
        public static int MAX_PLC_DATALENGTH = 10000;
        [StructLayout(LayoutKind.Sequential, Pack = 1)]

        public struct LoadCellData
        {
            public UInt32 datalength;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 15)]
            public float[] markdata;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 1024 * 40)]
            public float[] rawdata;
            public float keyindex;
        }

        public struct PLCRotateData
        {
            public UInt32 plcflen;
            //  [MarshalAs(UnmanagedType.ByValArray, SizeConst = 15)]
            //  public float[] plcmarkdata;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 10000)]
            public int[] plcy;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 10000)]
            public int[] plcx;
            // public float plckeyindex;
        }

        private LoadCellData loadcell;
        private PLCRotateData jycircle;
        private int flag_compare = 0;
        //private Color[] line_color = { Color.SpringGreen, Color.Red, Color.Blue, Color.Green, Color.DeepPink, Color.Purple, Color.Black, Color.Gold, Color.DarkOrange, Color.Silver };

        public CurveDrawer()
        {
            jycircle.plcx = new int[MAX_PLC_DATALENGTH];
            jycircle.plcy = new int[MAX_PLC_DATALENGTH];
            loadcell.markdata = new float[MARK_POINT_NUM];
            loadcell.rawdata = new float[MAX_RAW_DATALENGTH];
            InitializeComponent();
        }

        private void singleFileToolStripMenuItem_Click_1(object sender, EventArgs e)
        {
            button_Browse_Click(sender, e);
        }

        private void comboBox1_SelectionChangeCommitted(object sender, EventArgs e)
        {
            Console.WriteLine(comboBox1.SelectedText);
            string fName = comboBox1.SelectedText;
            OpenFileAndDraw(fName);
        }

        private void button_Browse_Click(object sender, EventArgs e)
        {
            string fName = "";

            OpenFileDialog openFile = new OpenFileDialog();
            openFile.InitialDirectory = "D:\\";//"D:\\KeyFeelingDemoData\\"
            openFile.Filter = "所有文件|*.*|CSV文件|*.csv|数据文件|*.data|文本文件|*.txt|C#文件|*.cs";
            openFile.RestoreDirectory = true;
            openFile.FilterIndex = 1;
            if (openFile.ShowDialog() == DialogResult.OK)
            {
                fName = openFile.FileName;

                DataTable combox_name = new DataTable();
                combox_name.Columns.Add("name");
                DataRow name_detail = combox_name.NewRow();
                name_detail[0] = fName;
                combox_name.Rows.Add(name_detail);

                comboBox1.DisplayMember = "name";
                comboBox1.DataSource = combox_name;

                OpenFileAndDraw(fName);
            }
        }
   
        private void multipleFilesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Compare_Click(sender, e);
        }

        private void Compare_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFiles = new OpenFileDialog();
            openFiles.InitialDirectory = "D:\\";//"D:\\KeyFeelingDemoData\\"
            openFiles.Multiselect = true;
            openFiles.Filter = "所有文件|*.*|CSV文件|*.csv|数据文件|*.data|文本文件|*.txt|C#文件|*.cs";
            openFiles.RestoreDirectory = true;
            openFiles.FilterIndex = 1;

            if (openFiles.ShowDialog() == DialogResult.OK)
            {
                string[] fNames = openFiles.FileNames;
                int filecounts = fNames.Length;
                PLCData.Visible = false;
                chart1.Visible = false;
                chart2.Visible = true;
                dataGridView1.Visible = false;
                if (filecounts > 50)
                {
                    MessageBox.Show("Filecounts > 50 !");
                }
                else
                {
                    DataTable combox_name = new DataTable();
                    combox_name.Columns.Add("name");
                    for(int i = 0; i < filecounts; i++)
                    {
                        DataRow name_detail = combox_name.NewRow();
                        name_detail[0] = fNames[i];
                        combox_name.Rows.Add(name_detail);
                    }
                    comboBox1.DisplayMember = "name";
                    comboBox1.DataSource = combox_name;
                    
                    for (int i = 0; i < filecounts; i++)
                    {
                        int str_flen = 1;
                        float[] markpoint = new float[14];
                        float[] x = new float[10000];
                        float[] y = new float[10000];

                        //read way
                        if (fNames[i].Contains(".csv"))
                        {
                            if (fNames[i].Contains("loadcell"))
                            {
                                flag_compare = 2;
                            }
                            else if (fNames[i].Contains("JoyStick_Circle"))
                            {
                                flag_compare = 1;
                            }
                            string[] str_line = System.IO.File.ReadAllLines(fNames[i]);
                            Read_csv(ref x, ref y, ref str_flen, ref markpoint, str_line);
                        }
                        else if (fNames[i].Contains(".data"))
                        {
                            if (fNames[i].Contains("unzip"))
                            {
                                Read_zipdata(fNames[i], ref x, ref y, ref str_flen, ref markpoint, 1);
                            }
                            else
                            {
                                Read_zipdata(fNames[i], ref x, ref y, ref str_flen, ref markpoint, 0);
                            }
                        }

                        //draw lins
                        if (fNames[i].Contains("JoyStick_Circle"))
                        {
                            Draw_Comparelines(x, y, str_flen, i);
                        }
                        else if (fNames[i].Contains("loadcell"))
                        {
                            Draw_Comparelines(x, y, str_flen, i);
                        }
                    }
                    flag_compare = 0;
                }
            }
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Form2 f = new Form2();
            f.Show();
        }

        private void quitToolStripMenuItem_Click_1(object sender, EventArgs e)
        {
            if (MessageBox.Show("Do you want to quit？", "Quit Warning", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes)
            {
                this.Close();
            }
        }
       
        private void CurveDrawer_Load(object sender, EventArgs e)
        {

        }

        public void Draw_Comparelines(float[] x, float[] y, int flen, int fileindex)
        {
            chart2.Series[fileindex].Points.Clear();
            chart2.Series[fileindex].Color = getcolor(fileindex);
            for (int j = 0; j < flen - 2; j++)
            {
                chart2.Series[fileindex].Name = fileindex.ToString();
                chart2.Series[fileindex].Points.AddXY(x[j], y[j] + fileindex * 10);
            }
        }

        public void OpenFileAndDraw(string fName)
        {
            int str_flen = 1;
            float[] markpoint = new float[14];
            float[] x = new float[10000];
            float[] y = new float[10000];

            //read way
            if (fName.Contains(".csv"))
            {
                string[] str_line = System.IO.File.ReadAllLines(fName);
                Read_csv(ref x, ref y, ref str_flen, ref markpoint, str_line);
            }
            else if (fName.Contains(".data"))
            {
                if (fName.Contains("unzip"))
                {
                    Read_zipdata(fName, ref x, ref y, ref str_flen, ref markpoint, 1);
                }
                else
                {
                    Read_zipdata(fName, ref x, ref y, ref str_flen, ref markpoint, 0);
                }
            }

            //draw line;
            if (str_flen > 10)
            {
                if (fName.Contains("JoyStick_Circle"))
                {
                    Draw_line_jy(x, y, str_flen, fName);
                }
                else if (fName.Contains("loadcell"))
                {
                    Draw_line_lc(x, y, str_flen, markpoint, fName);
                }
            }
            else
            {
                MessageBox.Show("The Data is empty !", "ERROR");
            }
            flag_compare = 0;
        }
        public void Draw_line_lc(float[] x, float[] y, int str_flen, float[] markpoint, string fName)
        {
            PLCData.Visible = false;
            chart1.Visible = true;
            chart2.Visible = false;
            dataGridView1.Visible = true;
            chart1.Series[0].Points.Clear();
            chart1.Series[1].Points.Clear();
            chart1.Series[2].Points.Clear();
            chart1.Series[3].Points.Clear();
            chart1.Series[4].Points.Clear();
            chart1.Series[5].Points.Clear();

            string rawdataname = fName;
            for (int j = 0; j < str_flen - 1; j++)
            {
                chart1.Series[0].Name = rawdataname;
                chart1.Series[0].Points.AddXY(x[j], y[j]);
            }

            if (markpoint[0] > 0)
            {
                chart1.Series[1].Points.AddXY(0.2, markpoint[0]);//mark 1
            }
            if (markpoint[3] > 0 && markpoint[2] > 0)
            {
                chart1.Series[1].Points.AddXY(markpoint[3], markpoint[2]);//mark 3,4
                chart1.Series[2].Points.AddXY(0, 0);//line2
                chart1.Series[2].Points.AddXY(markpoint[3], markpoint[2]);//line2
                chart1.Series[3].Points.AddXY(markpoint[3], markpoint[2]);//line7
                chart1.Series[3].Points.AddXY(markpoint[3], markpoint[4]);//line7
            }
            if (markpoint[4] > 0 && markpoint[5] > 0)
            {
                chart1.Series[1].Points.AddXY(markpoint[5], markpoint[4]);//mark 5,6
                chart1.Series[4].Points.AddXY(markpoint[3], markpoint[2]);//line8
                chart1.Series[4].Points.AddXY(markpoint[5], markpoint[4]);//line8
            }
            if (markpoint[9] > 0)
            {
                chart1.Series[1].Points.AddXY(markpoint[9], 80.0);//mark 10
                chart1.Series[5].Points.AddXY(markpoint[3], markpoint[2]);//line9
                chart1.Series[5].Points.AddXY(markpoint[9], markpoint[2]);//line9
            }
            if (markpoint[10] > 0)
            {
                chart1.Series[1].Points.AddXY(markpoint[10], 160.0);//mark 11
            }

            //table detail

            DataTable marktable = new DataTable();
            marktable.Columns.Add("name");
            marktable.Columns.Add("value");
            dataGridView1.DataSource = marktable;
            string[] nametable = { "force at 0.2mm", "Slope before Peak", "Peak force", "Peak travel", "Bottom force", "Bottom travel", "Drop Force", "Slope peak botton", "Drop Stroke", "Travel at 80g", "Travel at 160g", "Area under curve", "Ratio", "Close point travel", "Close point force" };
            for (int i = 0; i < 14; i++)
            {
                DataRow tb = marktable.NewRow();
                tb[0] = nametable[i];
                tb[1] = markpoint[i];
                marktable.Rows.Add(tb);       
            }
            for (int i = 0; i < this.dataGridView1.Columns.Count; i++)
            {
                this.dataGridView1.Columns[i].SortMode = DataGridViewColumnSortMode.NotSortable;
            }
        }

        public void Draw_line_jy(float[] x, float[] y, int str_flen, string fName)
        {
            PLCData.Visible = true;
            chart1.Visible = false;
            chart2.Visible = false;
            dataGridView1.Visible = false;
            PLCData.Series[0].Points.Clear();
            string rawdataname = fName;
            for (int j = 0; j < str_flen - 1; j++)
            {
                PLCData.Series[0].Name = rawdataname;
                PLCData.Series[0].Points.AddXY(x[j], y[j]);
            }
        }

        public void Read_csv(ref float[] x, ref float[] y, ref int str_flen, ref float[] markpoint, string[] str_line)
        {
            string[] str_line_coorder = new string[str_line.Length - 9];

            Regex reg_flen = new Regex(@"FeelingCurve Data longht:(\d+)");
            Match match_flen = reg_flen.Match(str_line[7]);
            if (match_flen.Success)
            {
                str_flen = int.Parse(match_flen.Groups[1].Value);
               // System.Console.WriteLine("flen={0};m={1};m.groups={2}", str_flen, match_flen, match_flen.Groups[1].Value);
                if (str_line.Length == 9 + str_flen)
                {
                    Array.Copy(str_line, 9, str_line_coorder, 0, str_line.Length - 9);
                    string str1 = string.Join(",", str_line_coorder);
                    string[] str2 = str1.Split(',');
                    for (int coorderindex = 0; coorderindex < str_flen; coorderindex++)
                    {
                        x[coorderindex] = float.Parse(str2[coorderindex * 2]);
                        y[coorderindex] = float.Parse(str2[coorderindex * 2 + 1]);
                        // Console.WriteLine("index = {0};x={1};y={2}", coorderindex, x[coorderindex], y[coorderindex]);
                    }

                    Regex reg_mark = new Regex(@"Test MarketData:,([\d\.]+),([\d\.]+),([\d\.]+),([\d\.]+),([\d\.]+),([\d\.]+),([\d\.]+),([\d\.]+),([\d\.]+),([\d\.]+),([\d\.]+),([\d\.]+),([\d\.]+),([\d\.]+)");
                    Match match_mark = reg_mark.Match(str_line[6]);
                    if (match_mark.Success)
                    {
                        for (int match_markindex = 0; match_markindex < 14; match_markindex++)
                        {
                            markpoint[match_markindex] = float.Parse(match_mark.Groups[match_markindex + 1].Value);
                            //System.Console.WriteLine(markpoint[match_markindex]);
                        }
                    }
                    else
                    {
                        MessageBox.Show("csv data error!\r  Can't get mark point data!");
                    }
                }
                else
                {
                    MessageBox.Show("csv data error!\r Data length wrong!");
                }
            }
            else
            {
                MessageBox.Show("csv data error!\r Can't get data length");
            }
        }

        public void Read_zipdata(string fName, ref float[] x, ref float[] y, ref int str_flen, ref float[] markpoint, int zip)
        {
            //获取文件内容
            FileStream fs = new FileStream(fName, FileMode.Open);
            byte[] tmpbyte = new byte[fs.Length];
           // System.Console.WriteLine("file length is: {0}", fs.Length);
           
            Int32 count = 0;
            count = fs.Read(tmpbyte, 0, (int)fs.Length);//文件读取byte-tmpbyte，count为数据大小
            fs.Close();
            byte[] gzip = new byte[1024 * 1024];
            int gziplength = 0;

            //解压数据
            if (zip == 0)
            {
                gzip = Decompress(tmpbyte, ref gziplength);
               // System.Console.WriteLine("length = {0} ; gzip = {1} ; gziplength = {2}", gziplength, gzip, gzip.Length);
            }
            if (fName.Contains("JoyStick_Circle"))
            {
                if (flag_compare == 2)
                {
                    MessageBox.Show("You have already opened loadcell data !\r Can't open different data ! ", "ERROR");
                }
                else
                {
                    jycircle.plcflen = 10;
                    jycircle.plcx = new int[MAX_PLC_DATALENGTH];
                    jycircle.plcy = new int[MAX_PLC_DATALENGTH];
                    if (zip == 0)
                    {
                        jycircle = (PLCRotateData)ByteToStruct(gzip, typeof(PLCRotateData), gziplength);
                    }
                    else if (zip == 1)
                    {
                        jycircle = (PLCRotateData)ByteToStruct(tmpbyte, typeof(PLCRotateData), -1);
                    }

                    str_flen = (int)jycircle.plcflen;
                    for (int i = 0; i < str_flen; i++)
                    {
                        x[i] = jycircle.plcx[i];
                        y[i] = jycircle.plcy[i];
                        // System.Console.WriteLine("plcdata:{0}:x={1};y={2}", i, jycircle.plcx[i], jycircle.plcy[i]);
                    }
                    flag_compare = 1;
                }
            }
            else if (fName.Contains("loadcell"))
            {
                if (flag_compare == 1)
                {
                    MessageBox.Show("You have already opened joystick data !\r Can't open different data ! ", "ERROR");
                }
                else
                {
                    loadcell.datalength = 10;
                    loadcell.markdata = new float[MARK_POINT_NUM];
                    loadcell.rawdata = new float[MAX_RAW_DATALENGTH];

                    if (zip == 0)
                    {
                        loadcell = (LoadCellData)ByteToStruct(gzip, typeof(LoadCellData), gziplength);
                    }
                    else if (zip == 1)
                    {
                        loadcell = (LoadCellData)ByteToStruct(tmpbyte, typeof(LoadCellData), -1);
                    }

                    //System.Console.WriteLine("file length is: {0}", loadcell.datalength);
                    str_flen = (int)(loadcell.datalength / 4 - 15) / 2;
                    for (int i = 0; i < str_flen; i++)
                    {
                        x[i] = loadcell.rawdata[i + str_flen];
                        y[i] = loadcell.rawdata[i];
                        //Console.WriteLine("plcdata:{0}:x={1};y={2}", i, loadcell.rawdata[i + str_flen], loadcell.rawdata[i]);
                    }
                    for (int i = 0; i < 14; i++)
                    {
                        markpoint[i] = loadcell.markdata[i];
                    }
                    flag_compare = 2;
                }
            }
        }

        public static Color getcolor(int fileindex)
        {
            Color line_color = new Color();
            // ColorTranslator.ToHtml(Color.FromArgb(255,255,255));得到的string
            int red = 55;
            int green = 55;
            int blue = 55;
            if (fileindex % 2 == 0)
            {
                getblue(fileindex, ref blue, ref green);
                red = 55 * (fileindex % 3 + 1);
                line_color = Color.FromArgb(60, red, green, blue);
            }
            else
            {
                getblue(fileindex, ref blue, ref green);
                red = 55 * (fileindex % 3 + 1);
                line_color = Color.FromArgb(165, red, green, blue);
            }
            return line_color;
        }
        public static void getblue(int fileindex, ref int blue, ref int green)
        {
            if (fileindex / 9 < 1)
            {
                blue = 55;
                getgreen(fileindex, ref green);

            }
            else if (fileindex / 9 >= 2)
            {
                blue = 165;
                getgreen(fileindex, ref green);
            }
            else
            {
                blue = 110;
                getgreen(fileindex, ref green);
            }
        }

        public static void getgreen(int fileindex, ref int green)
        {
            if (fileindex / 3 < 1)
            {
                green = 55;
            }
            else if (fileindex / 3 >= 2)
            {
                green = 165;
            }
            else
            {
                green = 110;
            }
        }


        public static byte[] Decompress(byte[] tmpbyte, ref int counts)
        {
            byte[] buf = new byte[1024 * 1024];
            MemoryStream ms = new MemoryStream(tmpbyte);
            GZipStream decompressedStream = new GZipStream(ms, CompressionMode.Decompress, true);
            counts = decompressedStream.Read(buf, 0, buf.Length);
            System.Console.WriteLine("count = {0} ; buf = {1} ; buflength = {2}", counts, buf, buf.Length);
            return buf;
        }
        public static object ByteToStruct(byte[] bytes, Type type, int byteslength)
        {
            int size = Marshal.SizeOf(type);
            if (size > bytes.Length)
            {
                //return null;
            }
            //分配结构体内存空间
            IntPtr structPtr = Marshal.AllocHGlobal(size);
            //将byte数组拷贝到分配好的内存空间
            if (byteslength < 0)
            {
                Marshal.Copy(bytes, 0, structPtr, bytes.Length);
            }
            else
            {
                Marshal.Copy(bytes, 0, structPtr, byteslength);
            }
            //将内存空间转换为目标结构体
            object obj = Marshal.PtrToStructure(structPtr, type);
            //释放内存空间
            Marshal.FreeHGlobal(structPtr);
            return obj;
        }

        private void text_FilePath_DoubleClick(object sender, EventArgs e)
        {
            button_Browse_Click(sender , e);
        }

  


    }
}
