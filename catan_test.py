import build.catan as catan
import unittest

class TestBoardCell(unittest.TestCase):
    def setUp(self):
        self.cell = catan.BoardCell(0,0,0,"O")

    def test_getFunctions(self):
        self.assertEqual(0,self.cell.getRow())
        self.assertEqual(0,self.cell.getCol())
        self.assertEqual(0,self.cell.getId())
        self.assertEqual(0,self.cell.getOwner())
        self.assertEqual("O",self.cell.getType())
        self.assertEqual(catan.Undeveloped,self.cell.getBuilding())
        self.assertEqual(catan.NoHarbor,self.cell.getHarbor())
        self.assertEqual([],self.cell.getAdjacent())

    def test_setFunctions(self):
        self.cell.setOwner(1)
        self.cell.setHarbor(catan.GenericHarbor)
        self.cell.setBuilding(catan.Settlement)
        self.cell.setAdjacent([1,2])
        self.assertEqual(1,self.cell.getOwner())
        self.assertEqual(catan.GenericHarbor,self.cell.getHarbor())
        self.assertEqual(catan.Settlement,self.cell.getBuilding())
        self.assertEqual([1,2],self.cell.getAdjacent())

class TestTile(unittest.TestCase):
    def setUp(self):
        self.adj = [0,1,2,11,12,13]
        self.tile = catan.Tile(0,0,0,4,catan.Hill,self.adj)

    def test_adjacencies(self):
        self.assertTrue(False)

    def test_hasRobber(self):
        self.assertFalse(self.tile.hasRobber())

    def test_getResources(self):
        # List with key as owner and value as number of resources.
        self.assertTrue(False)

    def test_getFunctions(self):
        self.assertEqual(0,self.tile.getRow())
        self.assertEqual(0,self.tile.getCol())
        self.assertEqual(0,self.tile.getId())
        self.assertEqual(4,self.tile.getNumber())
        self.assertEqual(self.adj,self.tile.getAdjacencies())
        self.assertEqual(catan.Hill,self.tile.getTerrain())

    def test_setFunctions(self):
        self.tile.setRobber(True)
        self.assertTrue(self.tile.hasRobber())

class TestRoad(unittest.TestCase):
    def setUp(self):
        self.cell1 = 0
        self.cell2 = 1
        self.road = catan.Road(self.cell1, self.cell2)

    def test_isAdjacent(self):
        self.assertTrue(False)
        self.assertFalse(True)

    def test_getCell(self):
        self.assertEqual(0,self.road.getCell(1))
        self.assertEqual(1,self.road.getCell(2))
        self.assertEqual(-1,self.road.getCell(3))
        self.assertEqual(-1,self.road.getCell(0))

    def test_getFunctions(self):
        self.assertEqual(0,self.road.getOwner())

    def test_setFunctions(self):
        self.road.setOwner(1)
        self.assertEqual(1,self.road.getOwner())

class TestBoard(unittest.TestCase):
    def setUp(self):
        self.board = catan.Board()
        self.board.loadBoardConfig("../catan_board.csv")
        self.board.calcAdjacencies()
        self.board.generateRoads()

    def test_loadDataConfig(self):
        self.assertTrue(False)

    def test_loadBoardConfig(self):
        self.assertEqual("O", self.board.getBoardCell(0,0).getType())
        self.assertEqual("P1", self.board.getBoardCell(1,1).getType())
        self.assertEqual("B", self.board.getBoardCell(2,2).getType())
        self.assertEqual("P5", self.board.getBoardCell(2,10).getType())

    def test_calcAdjacencies(self):
        sum = 0
        for i in range(6):
            for j in range(11):
                sum += len(self.board.getBoardCell(i,j).getAdjacent())
        self.assertEqual(72,int(sum/2))
        
        #Ocean Cell
        self.assertEqual([],self.board.getBoardCell(0,0).getAdjacent())

        #Non Ocean Cell
        self.assertEqual([37,25,27],self.board.getBoardCell(2,4).getAdjacent())

        #Center Cells
        self.assertEqual([12,22,24],self.board.getBoardCell(2,1).getAdjacent())
        self.assertEqual([35,23,25],self.board.getBoardCell(2,2).getAdjacent())

        #Top Cells
        self.assertEqual([13,3],self.board.getBoardCell(0,2).getAdjacent())
        self.assertEqual([2,4],self.board.getBoardCell(0,3).getAdjacent())

        #Bottom Cels
        self.assertEqual([46, 58],self.board.getBoardCell(5,2).getAdjacent())
        self.assertEqual([57, 59],self.board.getBoardCell(5,3).getAdjacent())

        #Left Cells
        self.assertEqual([33, 23],self.board.getBoardCell(2,0).getAdjacent())
        self.assertEqual([22, 34],self.board.getBoardCell(3,0).getAdjacent())

        #Right Cells
        self.assertEqual([43, 31],self.board.getBoardCell(2,10).getAdjacent())
        self.assertEqual([32, 42],self.board.getBoardCell(3,10).getAdjacent())

    def test_generateTiles(self):
        #Place Robber on First Desert
        self.assertTrue(False)

    def test_generateRoads(self):
        # Check if Road in list
        # Add Road if not in list
        self.assertEqual(0,1)

    def test_calcLongestRoad(self):
        self.assertTrue(False)

    def test_addRoad(self):
        #Check isValidRoad
        #Check no owner
        self.board.addRoad(0,1,1)
        self.assertEqual([0]*72,self.board.getRoads())
        self.board.addRoad(2,13,1)
        self.assertEqual(1,self.board.getRoads()[0])
        self.board.addRoad(2,13,2)
        self.assertEqual(1,self.board.getRoads()[0])

    def test_addSettlement(self):
        #check isTooClose
        #check for harbor
        #update boardcell
        self.assertTrue(False)

    def test_upgradeSettlement(self):
        #check for settlement
        #update boardcell
        self.assertTrue(False)

    def test_moveRobber(self):
        #Remove Robber from previous space
        #Add robber to new space.
        self.assertTrue(False)

    def test_isTooClose(self):
        #Nearest Building < 2 road lengths away.
        self.assertTrue(False)

    def test_hasRoad(self):
        #Check if road between two cells.
        self.assertTrue(False)

    def test_getBoard(self):
        board = []
        for i in range(6):
            a = []
            for j in range(11):
                b = []
                for k in range(2):
                    b = [0,0]
                a.append(b)
            board.append(a)

        self.assertEqual(6,len(self.board.getBoard()))
        for i in range(6):
            self.assertEqual(11,len(self.board.getBoard()[i]))
            for j in range(11):
                self.assertEqual(2,len(self.board.getBoard()[i][j]))
        self.assertEqual(board,self.board.getBoard())

    def test_getTiles(self):
        self.assertTrue(False)

    def test_getResources(self):
        # Combines Lists and Checks for Robber
        self.assertTrue(False)

    def test_getRoads(self):
        roads = [0]*72
        self.assertEqual(roads,self.board.getRoads())

    def test_getFunctions(self):
        self.assertEqual("../catan_board.csv",self.board.getFilename())

if __name__ == '__main__':
    unittest.main()