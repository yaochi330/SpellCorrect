#include <string>
#include <stdint.h>
#include <vector>

class EditDistance {
public:
	EditDistance(const std::string &ori_word, const std::string &key_word) :
			_edit_dis(100), _ori_word(ori_word), _key_word(key_word) {
	}
	int get_edit_dis() {
		return edit_distance(_ori_word, _key_word);
	}
private:
	int _edit_dis;
	const std::string _ori_word;
	const std::string _key_word;
	int MIN(int a, int b, int c) {
		int min = a;
		if (min > b)
			min = b;
		if (min > c)
			min = c;
		return min;
	}

	void convert(std::vector<uint16_t> &arr, const std::string &str) {
		std::string::const_iterator iter;
		int i = 0;
		for (iter = str.begin(); iter != str.end(); ++iter) {
			if (*iter & 0x80) {
				arr.push_back((*iter << 8) + *(++iter)); //为什么不能用按位或
			} else
				arr.push_back(*iter);
		}
	}

	int edit_distance(const std::string &str1, const std::string &str2) {
		//将字符串转换为数组
		std::vector<uint16_t> a;
		std::vector<uint16_t> b;
		convert(a, str1);
		convert(b, str2);

		int **mem = new int*[b.size() + 1];
		for (int i = 0; i <= b.size(); ++i) {
			mem[i] = new int[a.size() + 1];
		}
		//初始化其中第0行和第0列的全部元素
		for (int i = 0; i <= b.size(); ++i)
			mem[i][0] = i;
		for (int j = 0; j <= a.size(); ++j)
			mem[0][j] = j;

		for (int i = 1; i <= b.size(); ++i)
			for (int j = 1; j <= a.size(); ++j) {
				if (a[j - 1] == b[i - 1])
					mem[i][j] = mem[i - 1][j - 1];
				else
					mem[i][j] = MIN(mem[i - 1][j - 1] + 1, mem[i - 1][j] + 1,
							mem[i][j - 1] + 1);
			}
		int tmp = mem[b.size()][a.size()];
		for (int i = 0; i <= b.size(); ++i)
			delete[] mem[i];
		delete[] mem;
		return tmp;
	}
};
