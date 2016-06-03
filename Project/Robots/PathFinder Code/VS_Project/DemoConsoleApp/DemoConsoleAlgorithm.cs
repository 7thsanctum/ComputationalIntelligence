using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using PathFinderLibrary;

namespace DemoConsoleApplication
{
    /// <summary>
    /// A simple demonstration of an extension of an AbstractAlgorithm
    /// 
    /// The user must include
    /// 
    /// 1. A constructor that takes a board as a parameter and 
    ///    passes to the super class as below
    ///    
    /// 2. A run method where they will implement code
    /// 
    /// 3. Event listeners for the board_boardChanged and board_boardFinished events
    /// </summary>
    class DemoConsoleAlgorithm : AbstractAlgorithm
    {
        /// <summary>
        /// A constructor taking a Board as a parameter is required
        /// </summary>
        /// <param name="board"></param>
        
        public DemoConsoleAlgorithm(Board board):base(board)
        {

        }
               

       /// <summary>
       /// This method is required 
       /// Here a random direction is selected and attempt is made to move in that direction
       /// The process loops until either
       /// 1. The board is solved. 
       /// 2. The time has expired. 
       /// 3. The user has used up their allocated power. 
       /// Both these last two parameters are adjustable in the board designer
       /// </summary>
        public override void run()
        {
            Random rnd = new Random(Environment.TickCount);
            
            
            // Start the board and loop until either            
            
            Board.start();
            while(Board.isBoardRunning()){
                
                Direction direction = (Direction)rnd.Next(8);
                Board.move(direction);
                Cell cell = Board.getCurrentCell();
                Console.WriteLine(cell);
                Console.Write("Press any key to continue");
                Console.ReadKey();                
            }
        }

        /// <summary>
        /// This method responds to an event fired by the board on completion
        /// </summary>
        /// <param name="board"></param>
        public override void board_boardFinished(Board board)
        {
            //removing the listeners allows the algorithm to be executed again 
            board.boardChanged-=new Board.boardChangedHandler(board_boardChanged);
            board.boardFinished-=new Board.boardFinishedHandler(board_boardFinished);

            if (board.isBoardSolved())
            {
                Console.WriteLine("Board Finished and Solved");
            }
            else
            {
                Console.WriteLine("Board Finished but not Solved");
            }
            
        }

        /// <summary>
        /// This method responds to an event fired by the board after each move
        /// </summary>
        public override void board_boardChanged()
        {
            //Console.WriteLine("Board Changed");
        }
    }
}
