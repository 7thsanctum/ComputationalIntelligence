using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using PathFinderLibrary;

namespace DemoConsoleApplication
{
    class Program
    {
        /// <summary>
        ///  Load a board, pass it to a new algorithm instance and run it.
        ///  The IO operation will cause an error if the file is missing. 
        ///  Here the file is looked for in the same directory as the executable
        ///  usually \bin\Debug\ whilst developing
        /// </summary>
        static void Main()
        {
            Board board = BoardIO.getBoard("testMap.xml");
            AbstractAlgorithm alg = new DemoConsoleAlgorithm(board);
            alg.run();
        }
    }
}
