from build import catan
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
        self.tile = catan.Tile(0,0,0,catan.Hill,self.adj)
        self.tile.setNumber(4)

    def test_hasRobber(self):
        self.assertFalse(self.tile.hasRobber())
        self.tile.setRobber(True)
        self.assertTrue(self.tile.hasRobber())

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
        self.board = catan.Board("../catan_data.txt","../catan_board.csv",1)

    def test_loadDataConfig(self):
        self.assertEqual(6,self.board.getNumRows())
        self.assertEqual(11,self.board.getNumCols())
        self.assertEqual(19,self.board.getNumTiles())
        self.assertEqual(4,self.board.getMaxPlayers())
        self.assertEqual("O",self.board.getOceanSpot())
        self.assertEqual("P",self.board.getPortSpot())
        self.assertEqual("B",self.board.getBuildingSpot())
        self.assertEqual([0,3,7,12,16,17,18,15,11,6,2,1,4,8,13,14,10,5,9],self.board.getTileOrder())
        self.assertEqual([0,1,3,4,3,4,4],self.board.getTileCount())
        self.assertEqual([5,2,6,3,8,10,9,12,11,4,8,10,9,4,5,6,3,11],self.board.getTileNumbers())

    def test_loadBoardConfig(self):
        self.assertEqual("O", self.board.getBoardCell(0,0).getType())
        self.assertEqual("P1", self.board.getBoardCell(1,1).getType())
        self.assertEqual("B", self.board.getBoardCell(2,2).getType())
        self.assertEqual("P5", self.board.getBoardCell(2,10).getType())

    def test_badConfigFiles(self):
        with self.assertRaises(Exception) as context1:
            catan.Board("../not_exist.txt","../catan_board.csv")
        self.assertTrue("FileNotFoundException" in str(context1.exception))
        with self.assertRaises(Exception) as context2:
            catan.Board("../catan_data.txt","../not_exist.csv")
        self.assertTrue('FileNotFoundException' in str(context2.exception))
        with self.assertRaises(Exception) as context3:
            catan.Board("../catan_data.txt","../test/bad_board_1.csv",1)
        self.assertTrue("Board Size Inconsistent" in str(context3.exception))
        with self.assertRaises(Exception) as context4:
            catan.Board("../test/bad_data_1.txt","../catan_board.csv",1)
        self.assertEqual("Incorrect Value Type",str(context4.exception))
        with self.assertRaises(Exception) as context5:
            catan.Board("../test/bad_data_2.txt","../test/catan_board.csv",1)
        self.assertTrue("Incorrect Array Length" in str(context5.exception))

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
        self.assertEqual([0,1],self.board.getTiles()[1])
        #Does Tilecount Match
        tile_count = [0]*7
        for tile in self.board.getTiles():
            tile_count[tile[1]]+=1
        self.assertEqual([0,1,3,4,3,4,4],tile_count)
        self.assertTrue(self.board.hasRobber(1))
        self.assertEqual([22,23,24,33,34,35],self.board.getTile(7).getAdjacencies())
        self.assertEqual([12,13,14,23,24,25],self.board.getTile(3).getAdjacencies())

    def test_generateRoads(self):
        self.assertEqual(0,self.board.getRoads()[0])
        self.assertEqual(72,len(self.board.getRoads()))

    def test_calcRoadLengths(self):
        [6,7,8,17,18,19]
        self.assertEqual([0,0,0,0],self.board.calcRoadLength())
        self.board.addRoad(34,35,1)
        self.assertEqual([1,0,0,0],self.board.calcRoadLength())
        self.board.addRoad(36,35,1)
        self.board.addRoad(45,46,1)
        self.assertEqual([2,0,0,0],self.board.calcRoadLength())
        self.board.addRoad(47,46,1)
        self.board.addRoad(47,48,1)
        self.assertEqual([3,0,0,0],self.board.calcRoadLength())
        self.board.addRoad(36,47,1)
        self.assertEqual([5,0,0,0],self.board.calcRoadLength())
        self.board.addSettlement(3,3,2)
        self.assertEqual([3,0,0,0],self.board.calcRoadLength())
        self.board.addRoad(34,45,1)
        self.assertEqual([6,0,0,0],self.board.calcRoadLength())

        self.board.addRoad(6,7,2)
        self.board.addRoad(7,8,2)
        self.board.addRoad(8,19,2)
        self.board.addRoad(17,18,2)
        self.board.addRoad(18,19,2)
        self.board.addRoad(6,17,2)
        self.assertEqual([6,6,0,0],self.board.calcRoadLength())
        self.board.addSettlement(1,7,3)
        self.assertEqual([6,6,0,0],self.board.calcRoadLength())
        self.board.addRoad(18,29,3)
        self.assertEqual([6,6,1,0],self.board.calcRoadLength())

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
        self.assertEqual(catan.Undeveloped,self.board.getBoardCell(2,3).getBuilding())
        self.assertEqual(0,self.board.getBoardCell(2,3).getOwner())
        self.board.addSettlement(2,3,4)
        self.assertEqual(catan.Settlement,self.board.getBoardCell(2,3).getBuilding())
        self.assertEqual(4,self.board.getBoardCell(2,3).getOwner())
        self.board.addSettlement(2,2,4)
        self.assertEqual(catan.Settlement,self.board.getBoardCell(2,3).getBuilding())
        self.assertEqual(0,self.board.getBoardCell(2,2).getOwner())
        self.board.addSettlement(2,4,2)
        self.assertEqual(catan.Settlement,self.board.getBoardCell(2,3).getBuilding())
        self.assertEqual(0,self.board.getBoardCell(2,4).getOwner())
        self.board.addSettlement(1,3,3)
        self.assertEqual(catan.Settlement,self.board.getBoardCell(2,3).getBuilding())
        self.assertEqual(0,self.board.getBoardCell(1,3).getOwner())
        self.board.addSettlement(3,3,1)
        self.assertEqual(catan.Settlement,self.board.getBoardCell(2,3).getBuilding())
        self.assertEqual(1,self.board.getBoardCell(3,3).getOwner())

    def test_upgradeSettlement(self):
        self.board.upgradeSettlement(0,6)
        self.assertEqual(catan.Undeveloped,self.board.getBoardCell(0,6).getBuilding())
        self.board.addSettlement(0,6,1)
        self.assertEqual(catan.Settlement,self.board.getBoardCell(0,6).getBuilding())
        self.board.upgradeSettlement(0,6)
        self.assertEqual(catan.City,self.board.getBoardCell(0,6).getBuilding())

    def test_moveRobber(self):
        self.assertTrue(self.board.hasRobber(1))
        self.board.moveRobber(2)
        self.assertFalse(self.board.hasRobber(1))
        self.assertTrue(self.board.hasRobber(2))
        self.board.moveRobber(2)
        self.assertTrue(self.board.hasRobber(2))
        self.board.moveRobber(8)
        self.assertFalse(self.board.hasRobber(2))
        self.assertTrue(self.board.hasRobber(8))

    def test_isTooClose(self):
        #Nearest Building < 2 road lengths away.
        self.board.addSettlement(3,3,4)
        self.assertTrue(self.board.isTooClose(3,3))
        self.assertFalse(self.board.isTooClose(2,3))
        self.assertTrue(self.board.isTooClose(4,3))
        self.assertTrue(self.board.isTooClose(3,2))
        self.assertTrue(self.board.isTooClose(3,4))

    def test_hasRobber(self):
        self.assertTrue(self.board.hasRobber(1))
        self.assertFalse(self.board.hasRobber(8))

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
        tiles = [[5, 5],[0, 1],[8, 5],[2, 4],[10, 6],[3, 4],[4, 4],[6,3],[9, 5],[11, 5],[6, 6],[11,3],[3,6],[4,2],[5,6],[12,3],[8,2],[10,2],[9,3]]
        self.assertEqual(tiles,self.board.getTiles())

    def test_getTileIndex(self):
        self.assertEqual([1],self.board.getTileIndex(0))
        self.assertEqual([],self.board.getTileIndex(1))
        self.assertEqual([3],self.board.getTileIndex(2))
        self.assertEqual([7,10],self.board.getTileIndex(6))
        self.assertEqual([],self.board.getTileIndex(7))

    def test_getResources(self):
        # Combines Lists and Checks for Robber
        resources = [[0]*4 for _ in range(5)]
        self.assertEqual(0,self.board.getBoardCell(0,6).getOwner())
        self.assertEqual(resources,self.board.getResources(8))
        self.board.addSettlement(0,6,2)
        resources[3][1] = 1
        self.assertEqual(resources,self.board.getResources(8))
        self.board.addSettlement(0,8,1)
        self.assertEqual(catan.Settlement,self.board.getBoardCell(0,8).getBuilding())
        resources[3][0] = 1
        self.assertEqual(resources,self.board.getResources(8))
        self.board.addSettlement(4,2,2)
        resources[0][1] = 1
        self.assertEqual(resources,self.board.getResources(8))
        self.board.upgradeSettlement(0,6)
        resources[3][1] = 2
        self.assertEqual(resources,self.board.getResources(8))
        self.board.moveRobber(2)
        resources[3][1] = 0
        resources[3][0] = 0
        self.assertEqual(resources,self.board.getResources(8))

    def test_getRoads(self):
        roads = [0]*72
        self.assertEqual(roads,self.board.getRoads())

    def test_getRoadIndex(self):
        self.assertEqual(11,self.board.getRoadIndex(12,13))
        self.assertEqual(-1,self.board.getRoadIndex(12,20))
        self.assertEqual(-1,self.board.getRoadIndex(0,1))

    def test_getFunctions(self):
        self.assertEqual("../catan_data.txt",self.board.getDataConfigFile())
        self.assertEqual("../catan_board.csv",self.board.getBoardConfigFile())
        self.assertEqual('O',self.board.getBoardCell(0,0).getType())
        self.assertEqual('P6',self.board.getBoardCell(4,9).getType())
        self.assertEqual('B',self.board.getBoardCell(3,3).getType())
        self.assertEqual((0,0),self.board.getBoardCellCoords(0))
        self.assertEqual((3,6),self.board.getBoardCellCoords(39))
        self.assertEqual(int,type(self.board.getNumRows()))
        self.assertEqual(int,type(self.board.getNumCols()))
        self.assertEqual(int,type(self.board.getNumTiles()))
        self.assertEqual(int,type(self.board.getMaxPlayers()))
        self.assertEqual(str,type(self.board.getOceanSpot()))
        self.assertEqual(str,type(self.board.getPortSpot()))
        self.assertEqual(str,type(self.board.getBuildingSpot()))
        self.assertEqual(list,type(self.board.getTileOrder()))
        self.assertEqual(list,type(self.board.getTileCount()))
        self.assertEqual(list,type(self.board.getTileNumbers()))

if __name__ == '__main__':
    unittest.main()