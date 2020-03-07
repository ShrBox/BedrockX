#include "pch.h"
static std::unordered_map<void*, int> dim_id;
static uintptr_t bs_dim;
LBAPI WDim WBlockSource::getDim() {
	return *(WDim*)((uintptr_t)v + bs_dim);
}
LBAPI int WDim::getID() {
	return dim_id[v];
}
THook(void*, "??0BlockSource@@QEAA@AEAVLevel@@AEAVDimension@@AEAVChunkSource@@_N3@Z", void* a0, void* a1, void* a2_dim, void* a3, bool a4, bool a5) {
	auto rv = original(a0, a1, a2_dim, a3, a4, a5);
	if (!bs_dim) {
		void** pfind = (void**)a0;
		for (int poff = 1; poff < 8; ++poff) {
			if (pfind[poff] == a2_dim) {
				if (bs_dim) {
					printf("bs_dim wtf %lld %d\n", bs_dim, poff * 8);
				}
				bs_dim = poff * 8;
			}
		}
	}
	return rv;
}
THook(void*, "??0Dimension@@QEAA@AEAVLevel@@V?$AutomaticID@VDimension@@H@@FAEAVScheduler@@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z", void* a0_dim, void* a1, int a2_id, void* a3, void* a4, void* a5) {
	auto rv = original(a0_dim, a1, a2_id, a3, a4, a5);
	dim_id[a0_dim] = a2_id;
	return rv;
}

LBAPI std::unique_ptr<BlockSource> WDim::makeSource() {
	return std::make_unique<BlockSource>(LocateS<ServerLevel>(), *v, LocateS<ChunkSource>(), 1, 0);
}
LBAPI void WDim::setBlock(int x, int y, int z, Block const& blk) {
	BlockSource bs(LocateS<ServerLevel>(), *v, LocateS<ChunkSource>(), 1, 0);
	bs.setBlock({ x, y, z }, blk, 3, nullptr);
	//stub
}
LBAPI struct WBlock WDim::getBlock(int x, int y, int z) {
	BlockSource bs(LocateS<ServerLevel>(), *v, LocateS<ChunkSource>(), 1, 0);
	return WBlock(bs.getBlock(x, y, z));
}