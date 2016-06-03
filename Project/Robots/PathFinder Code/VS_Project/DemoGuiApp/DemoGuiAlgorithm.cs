using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using PathFinderLibrary;
using System.Threading;

namespace DemoGuiApp
{
    public class DemoGuiApplication : AbstractAlgorithm
    {
        List<Direction> dirList = new List<Direction>();

        List<Direction> shortestRoute = new List<Direction>();

        // A genome in this case will consist of direction and number of times to move in that direction
        List<List<Direction>> genome = new List<List<Direction>>();
        // ######
        int populationSize = 14;
        int chromosomeLength = 20;
        void PopulateGenes()
        {
            Random myRandom = new Random();

            for (int i = 0; i < populationSize; i++)
            {
                genome.Add(new List<Direction>());
                for (int j = 0; j < chromosomeLength; j++)
                {
                    genome.Last<List<Direction>>().Add(dirList[myRandom.Next(0, 7)]);
                }
            }

            int val = Board.solve(genome[0]);

        }

        /// <summary>
        /// Required Constructor passes the board to base AbstractAlgorithm class
        /// </summary>
        /// <param name="board">Required: The board to be solved</param>
        public DemoGuiApplication(Board board)
            : base(board)
        {
            dirList.Add(Direction.North);
            dirList.Add(Direction.NorthEast);
            dirList.Add(Direction.East);
            dirList.Add(Direction.SouthEast);
            dirList.Add(Direction.South);
            dirList.Add(Direction.SouthWest);
            dirList.Add(Direction.West);
            dirList.Add(Direction.NorthWest);
            dirList.Add(Direction.Undefined);

            PopulateGenes();
        }

        

        /// <summary>
        /// Overrides required run method. 
        /// The user code for solving the board should be implemented here
        /// </summary>
        public override void run()
        {
            Random rnd = new Random(Environment.TickCount);
            while (Board.isBoardRunning())
            {
                int min = Board.getMinimumMovesToEnd(Board.getCurrentCell());

                int val = rnd.Next(8);
                val = 4;
                List<Direction> freeCells = new List<Direction>();
                foreach (Direction d in dirList)
                {
                    Cell adjCell = Board.getCell(d, 1);
                    if (adjCell != null)
                    {
                        if (!adjCell.IsBlocking)
                        {
                            freeCells.Add(d);
                        }
                    }
                }

                Direction toMove = Direction.Undefined;
                double prevEuclidDistance = 1000;

                foreach (Direction d in freeCells)
                {
                    Cell adjCell = Board.getCell(d, 1);

                    if (adjCell.VisitedCount < Board.getCurrentCell().VisitedCount && Board.getEuclideanDistanceToEnd(Board.getCell(d, 1)) < prevEuclidDistance)
                    {
                        toMove = d;
                    }

                    prevEuclidDistance = Board.getEuclideanDistanceToEnd(Board.getCell(d, 1));
                }

                Board.move(toMove);
                Cell cell = Board.getCurrentCell();
                System.Console.WriteLine(cell.ToString());
            }

        }
        /*
        public override void run()
        {         
            Random rnd = new Random(Environment.TickCount);             
            while (Board.isBoardRunning())
            {
                int min = Board.getMinimumMovesToEnd(Board.getCurrentCell());

                int val = rnd.Next(8);
                val = 4;
                List<Direction> freeCells = new List<Direction>();
                foreach (Direction d in dirList)
                {
                    Cell adjCell = Board.getCell(d, 1);
                    if (adjCell != null)
                    {
                        if (!adjCell.IsBlocking)
                        {
                            freeCells.Add(d);
                        }
                    }
                }

                Direction toMove = Direction.Undefined;
                double prevEuclidDistance = 1000;

                foreach (Direction d in freeCells)
                {
                    Cell adjCell = Board.getCell(d, 1);

                    if (adjCell.VisitedCount < Board.getCurrentCell().VisitedCount && Board.getEuclideanDistanceToEnd(Board.getCell(d, 1)) < prevEuclidDistance)
                    {
                        toMove = d;
                    }

                    prevEuclidDistance = Board.getEuclideanDistanceToEnd(Board.getCell(d, 1));
                }
                
                Board.move(toMove);
                Cell cell = Board.getCurrentCell();
                System.Console.WriteLine(cell.ToString());                                
            }

        }
        */
        /// <summary>
        /// Required but not used. Mainly for console application use.
        /// 
        /// Executed when the board is complete
        /// </summary>
        /// <param name="board"></param>
        public override void board_boardFinished(Board board)
        {            
        }
        /// <summary>
        /// Required but not used. Mainly for console application use
        /// 
        /// Responds to an event fired by the board after each move
        /// </summary>
        public override void board_boardChanged()
        {            
        }
    }
}
