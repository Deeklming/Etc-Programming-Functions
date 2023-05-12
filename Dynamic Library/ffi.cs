using static System.Console;
using System.Runtime.InteropServices;
using System.Diagnostics;
namespace CsStudy{
    class Program{
        [DllImport(@".\ffi_thread.dll")]
        public static extern void process();
        static void Main(string[] args){
            Stopwatch stopwatch = new Stopwatch();
            stopwatch.Start();
 
            process();

            stopwatch.Stop();
            WriteLine($"측정시간: {stopwatch.ElapsedMilliseconds}ms");
        }
    }
}
