#ifndef MEMORY_H
#define MEMORY_H
#include <vector>
#include <memory>
#include <algorithm>
#include <numeric>
#include <iostream>
namespace osLab {
	namespace memory {
		namespace successive {
			typedef struct pieceMes {
				int pieceTag;
				int Length;
				int useLength;
				bool allocateStatus;
			}pieceMes;
			typedef enum alloctae_way {
				ALLOCTAE_FIXED,
				ALLOCTAE_MULTI
			}allocate_way;
			static bool operator<(const pieceMes& a, const pieceMes& b);
			static bool operator>=(const pieceMes& a, const pieceMes& b);
			class Physical_Memory {
				private:
					//by byte
					std::vector<pieceMes> allocMes;
				public:
					Physical_Memory(int memorysLength, allocate_way alo, void* data);
					void first_batch(int needSpace);
					void best_batch(int needSpace);
					void worst_batch(int needSpace);
					void for_batch(int needSpace);
			};
		}
	}
}
#endif