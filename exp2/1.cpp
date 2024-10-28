#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <cstdlib>
#include <ctime>

// 计算最大矩形面积的函数
int largestRectangleArea(const std::vector<int>& heights) {
    std::stack<int> indices;
    int max_area = 0;
    int n = heights.size();

    for (int i = 0; i <= n; ++i) {
        int current_height = (i == n) ? 0 : heights[i];
        
        while (!indices.empty() && heights[indices.top()] > current_height) {
            int height = heights[indices.top()];
            indices.pop();
            int width = indices.empty() ? i : i - indices.top() - 1;
            max_area = std::max(max_area, height * width);
        }
        indices.push(i);
    }

    return max_area;
}

// 随机生成测试数据
std::vector<int> generateRandomHeights(int n, int max_height) {
    std::vector<int> heights(n);
    for (int i = 0; i < n; ++i) {
        heights[i] = rand() % (max_height + 1);
    }
    return heights;
}

int main() {
    // 设置随机种子
    srand(static_cast<unsigned int>(time(0)));

    // 测试示例
    std::vector<int> test1 = {2, 1, 5, 6, 2, 3};
    std::cout << "示例 1: 输入: [2, 1, 5, 6, 2, 3] 输出: " << largestRectangleArea(test1) << std::endl; // 输出: 10

    std::vector<int> test2 = {2, 4};
    std::cout << "示例 2: 输入: [2, 4] 输出: " << largestRectangleArea(test2) << std::endl; // 输出: 4

    // 随机生成并测试10组数据
    std::cout << "随机生成的测试数据:\n";
    for (int i = 0; i < 10; ++i) {
        int n = rand() % 100000 + 1; // 随机生成1到10^5之间的柱子数量
        std::vector<int> heights = generateRandomHeights(n, 10000); // 随机生成柱子的高度，范围为0到10^4
        int area = largestRectangleArea(heights);
        std::cout << "测试 " << (i + 1) << ": 最大面积 = " << area << std::endl;
    }

    return 0;
}

