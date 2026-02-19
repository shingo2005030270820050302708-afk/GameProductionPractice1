//#include "BlockManager.h"
//#include "../Map/MapManager.h"
//#include "../Map/MapParameter.h"
//
//BlockManager g_BlockManager;
//
//void BlockManagerInit()
//{
//    blocks.clear();
//
//    auto map = GetMapData();
//
//    for (int y = 0; y < MAP_CHIP_Y_NUM; y++)
//    {
//        for (int x = 0; x < MAP_CHIP_X_NUM; x++)
//        {
//            MapChipData& chip = map[y][x];
//
//            if (chip.mapChip == BLOCK_SPAWN)
//            {
//                SpawnBlock(x * MAP_CHIP_WIDTH, y * MAP_CHIP_HEIGHT);
//            }
//        }
//    }
//}
//
//void SpawnBlock(int x, int y)
//{
//    blocks.emplace_back(x, y);
//}
//
//
//void BlockManagerDraw()
//{
//    for (auto& b : blocks)
//        if (b.alive)
//            b.Draw();
//}