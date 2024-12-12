# REPORT

## Phần 1: Giới thiệu chung và mục tiêu

    Báo cáo Chương trình Quản lý Sinh viên

### 1. Giới thiệu

        Chương trình quản lý sinh viên được xây dựng nhằm mục đích lưu trữ, cập nhật, tra cứu và xóa thông tin sinh viên một cách hiệu quả. Chương trình sử dụng hai cấu trúc dữ liệu chính là bảng băm (Hash Table) và cây AVL (AVL Tree) để tối ưu hóa hiệu suất cho các thao tác khác nhau. Thông tin sinh viên bao gồm: mã sinh viên (8 ký tự), họ tên, năm sinh, điểm trung tuyển và điểm trung bình.

### 2. Mục tiêu

        Lưu trữ thông tin sinh viên: Chương trình cho phép nhập thông tin sinh viên từ bàn phím và lưu trữ vào tệp "sinhvien.txt".

        Tra cứu nhanh: Cho phép tra cứu thông tin sinh viên theo mã sinh viên một cách nhanh chóng, sử dụng bảng băm.

        Cập nhật và xóa: Cho phép cập nhật điểm trung bình và xóa thông tin sinh viên.

        Duy trì tính toàn vẹn dữ liệu: Đảm bảo không có mã sinh viên trùng lặp trong hệ thống.

        Hiệu suất cao: Tối ưu hóa thời gian thực hiện các thao tác tra cứu, cập nhật và xóa thông tin.

### 3. Cấu trúc dữ liệu

        Chương trình sử dụng hai cấu trúc dữ liệu chính:

        Bảng băm (Hash Table): Dùng để tra cứu nhanh thông tin sinh viên theo mã sinh viên.

        Cây AVL (AVL Tree): Dùng để lưu trữ thông tin sinh viên, hỗ trợ việc cập nhật và xóa thông tin, đồng thời duy trì tính cân bằng để đảm bảo hiệu suất tìm kiếm.

## Phần 2: Bảng băm (Hash Table)

### 1. Vai trò

        Bảng băm được sử dụng để tra cứu nhanh thông tin sinh viên dựa vào mã sinh viên. Mỗi mã sinh viên sẽ được băm (hash) thành một chỉ số (index) trong bảng băm. Khi cần tra cứu, chương trình sẽ băm mã sinh viên cần tìm và truy cập trực tiếp vào vị trí tương ứng trong bảng băm.

### 2. Cấu trúc

        HashNode: Mỗi nút trong bảng băm (HashNode) lưu trữ:

        maSV: Mã sinh viên (khóa).

        data: Con trỏ đến đối tượng SinhVien chứa thông tin chi tiết của sinh viên.

        next: Con trỏ đến nút tiếp theo trong danh sách liên kết (dùng để giải quyết đụng độ).

        hashTable: Là một mảng các con trỏ HashNode, có kích thước cố định HASH_TABLE_SIZE (100000). Mỗi phần tử trong mảng có thể là nullptr (ô trống) hoặc trỏ đến một HashNode đầu tiên của danh sách liên kết.

### 3. Hàm băm (hashFunction)

        int hashFunction(string maSV) {
            int sum = 0;
            for (char c : maSV) {
                sum += c;
            }
            return sum % 100000; // Lấy hàm băm có kích thước 100000 (Tránh đụng độ)
        }

        Hàm băm đơn giản này tính tổng giá trị ASCII của các ký tự trong mã sinh viên, sau đó lấy phần dư của tổng này khi chia cho kích thước bảng băm (HASH_TABLE_SIZE). Mục đích là để phân phối các mã sinh viên vào các vị trí khác nhau trong bảng băm, giảm thiểu đụng độ.

### 4. Giải quyết đụng độ

        Chương trình sử dụng phương pháp Separate Chaining (Dây chuyền riêng biệt) để giải quyết đụng độ. Khi hai mã sinh viên khác nhau có cùng giá trị băm, chúng sẽ được lưu trữ trong cùng một danh sách liên kết tại vị trí tương ứng trong bảng băm.

### 5. Thao tác trên bảng băm

        Thêm sinh viên:

        Tính chỉ số băm của mã sinh viên bằng hashFunction.

        Tạo một HashNode mới chứa mã sinh viên và con trỏ đến dữ liệu SinhVien.

        Nếu ô trống tại chỉ số băm, gán HashNode mới vào vị trí đó.

        Nếu đã có danh sách liên kết tại chỉ số băm, thêm HashNode mới vào cuối danh sách.

        Tra cứu sinh viên:

        Tính chỉ số băm của mã sinh viên cần tìm.

        Duyệt qua danh sách liên kết tại vị trí đó.

        Nếu tìm thấy HashNode có maSV trùng khớp, trả về con trỏ đến dữ liệu SinhVien.

        Nếu không tìm thấy, trả về nullptr.

        Xóa sinh viên:

        Tính chỉ số băm của mã sinh viên cần xóa.

        Duyệt qua danh sách liên kết tại vị trí đó.

        Nếu tìm thấy HashNode có maSV trùng khớp:

        Xóa HashNode khỏi danh sách liên kết.

        Giải phóng bộ nhớ của HashNode và dữ liệu SinhVien.

        Nếu không tìm thấy, không làm gì cả.

## Phần 3: Cây AVL (AVL Tree)

### 1. Vai trò

        Cây AVL được sử dụng để lưu trữ thông tin sinh viên, hỗ trợ việc cập nhật và xóa thông tin một cách hiệu quả. Cây AVL là cây tìm kiếm nhị phân tự cân bằng, đảm bảo chiều cao của cây luôn ở mức logarit, giúp cho việc tìm kiếm, thêm và xóa phần tử có độ phức tạp thời gian là O(log n).

### 2. Cấu trúc

        AVLNode: Mỗi nút trong cây AVL (AVLNode) lưu trữ:

        data: Đối tượng SinhVien chứa thông tin chi tiết của sinh viên.

        left: Con trỏ đến cây con trái.

        right: Con trỏ đến cây con phải.

        height: Chiều cao của nút.

### 3. Các phép toán cơ bản trên cây AVL

        getHeight(AVLNode* node): Trả về chiều cao của nút.

        getBalanceFactor(AVLNode* node): Tính hệ số cân bằng của nút (chiều cao cây con trái trừ chiều cao cây con phải).

        rightRotate(AVLNode* y): Xoay phải tại nút y.

        leftRotate(AVLNode* x): Xoay trái tại nút x.

### 4. Thao tác trên cây AVL

        Thêm sinh viên (insertAVL):

        Thực hiện thêm nút như cây nhị phân tìm kiếm thông thường.

        Cập nhật chiều cao của các nút bị ảnh hưởng.

        Kiểm tra hệ số cân bằng của các nút từ nút mới thêm đến gốc.

        Nếu mất cân bằng (hệ số cân bằng > 1 hoặc < -1), thực hiện các phép xoay để cân bằng lại cây. Các trường hợp mất cân bằng và cách xoay:

        Left Left (LL): Xoay phải tại nút mất cân bằng.

        Right Right (RR): Xoay trái tại nút mất cân bằng.

        Left Right (LR): Xoay trái tại nút con trái, sau đó xoay phải tại nút mất cân bằng.

        Right Left (RL): Xoay phải tại nút con phải, sau đó xoay trái tại nút mất cân bằng.

        Tìm kiếm sinh viên (searchAVL):

        Thực hiện tìm kiếm như cây nhị phân tìm kiếm thông thường, dựa vào maSV.

        Trả về nút chứa SinhVien nếu tìm thấy, ngược lại trả về nullptr.

        Xóa sinh viên (deleteAVL):

        Thực hiện xóa nút như cây nhị phân tìm kiếm thông thường.

        Cập nhật chiều cao của các nút bị ảnh hưởng.

        Kiểm tra hệ số cân bằng của các nút từ nút cha của nút bị xóa đến gốc.

        Nếu mất cân bằng, thực hiện các phép xoay tương tự như khi thêm để cân bằng lại cây.

        Tìm nút có giá trị nhỏ nhất (minValueNode): Dùng trong trường hợp xóa nút có 2 con.

## Phần 4: Tích hợp Bảng băm và Cây AVL, và Kết luận

### 1. Tích hợp Bảng băm và Cây AVL

        Đồng bộ dữ liệu: Khi thêm, xóa hoặc cập nhật thông tin sinh viên, chương trình thực hiện thao tác trên cả bảng băm và cây AVL để đảm bảo tính nhất quán của dữ liệu.

        Thêm: Thêm vào cả bảng băm và cây AVL.

        Xóa: Xóa khỏi cả bảng băm và cây AVL.

        Cập nhật: Cập nhật trên cây AVL (tìm kiếm bằng searchAVL), sau đó cập nhật thông tin trên file và đọc lại file để cập nhật bảng băm.

        Tra cứu: Ưu tiên sử dụng bảng băm để tra cứu nhanh.

        Cập nhật và xóa: Sử dụng cây AVL để tìm kiếm và thực hiện thao tác, sau đó cập nhật bảng băm cho đồng bộ.

### 2. Ưu điểm của việc kết hợp

        Tra cứu nhanh: Bảng băm cung cấp khả năng tra cứu rất nhanh với độ phức tạp thời gian trung bình là O(1).

        Cập nhật và xóa hiệu quả: Cây AVL cho phép cập nhật và xóa thông tin với độ phức tạp thời gian là O(log n), tốt hơn so với việc tìm kiếm và cập nhật trực tiếp trên file (O(n)).

        Duy trì tính toàn vẹn: Việc không cho phép trùng mã sinh viên được đảm bảo nhờ cả bảng băm và cây AVL.

### 3. Hạn chế

        Độ phức tạp của code: Việc sử dụng hai cấu trúc dữ liệu phức tạp làm tăng độ phức tạp của mã nguồn.

        Bộ nhớ: Sử dụng đồng thời hai cấu trúc dữ liệu có thể tốn nhiều bộ nhớ hơn so với việc chỉ sử dụng một cấu trúc.

        Hàm băm chưa tối ưu: Hàm băm hiện tại khá đơn giản, có thể dẫn đến đụng độ nhiều trong một số trường hợp, ảnh hưởng đến hiệu suất tra cứu.

### 4. Kết luận

        Chương trình quản lý sinh viên đã đạt được các mục tiêu đề ra, cho phép lưu trữ, tra cứu, cập nhật và xóa thông tin sinh viên một cách hiệu quả. Việc kết hợp bảng băm và cây AVL giúp tận dụng được ưu điểm của cả hai cấu trúc dữ liệu, mang lại hiệu suất cao cho chương trình. Tuy nhiên, chương trình vẫn còn một số hạn chế có thể được cải thiện trong tương lai, ví dụ như sử dụng hàm băm tốt hơn, tối ưu hóa việc sử dụng bộ nhớ, và thêm các chức năng khác.

## Phần 5: Hoạt động chi tiết của Bảng băm

### 1. Thêm sinh viên vào bảng băm

        Khi thêm một sinh viên mới, chương trình thực hiện các bước sau:

        Tính toán chỉ số băm:

            Sử dụng hàm hashFunction(maSV) để tính toán chỉ số băm (index) dựa trên mã sinh viên.

            Ví dụ: maSV = "SV123456".

            sum = 'S' + 'V' + '1' + '2' + '3' + '4' + '5' + '6' = 83 + 86 + 49 + 50 + 51 + 52 + 53 + 54 = 528

            index = 528 % 100000 = 528

        Tạo HashNode mới:

            Tạo một HashNode mới chứa maSV, con trỏ data đến đối tượng SinhVien, và con trỏ next được khởi tạo là nullptr.

        Thêm HashNode vào bảng băm:

            Truy cập phần tử hashTable[index] (ví dụ: hashTable[528]).

            Trường hợp 1: Ô trống (hashTable[index] == nullptr): Gán HashNode mới vào vị trí này.

            Trường hợp 2: Đã có phần tử (hashTable[index] không nullptr): Xảy ra đụng độ.

                Duyệt qua danh sách liên kết bắt đầu từ hashTable[index].

                Tìm nút cuối cùng trong danh sách (nút có next == nullptr).

                Gán next của nút cuối cùng bằng HashNode mới.

        Ví dụ minh họa:

        Giả sử bảng băm ban đầu có các phần tử sau:

            hashTable[100] -> HashNode("SV000001", data1) -> nullptr

            hashTable[528] -> HashNode("SV000002", data2) -> nullptr

            hashTable[99999] -> HashNode("SV999999", data999999) -> nullptr

        Thêm sinh viên mới có maSV = "SV123456" (index = 528):

            Tính index = 528.

            Tạo HashNode mới: newNode("SV123456", data123456).

            hashTable[528] đã có phần tử, duyệt đến cuối danh sách.

            Gán hashTable[528]->next = newNode.

        Bảng băm sau khi thêm:

            hashTable[100] -> HashNode("SV000001", data1) -> nullptr

            hashTable[528] -> HashNode("SV000002", data2) -> HashNode("SV123456", data123456) -> nullptr

            hashTable[99999] -> HashNode("SV999999", data999999) -> nullptr

### 2. Xóa sinh viên khỏi bảng băm

        Khi xóa một sinh viên, chương trình thực hiện các bước sau:

            Tính toán chỉ số băm: Sử dụng hashFunction(maSV) để tính index dựa trên mã sinh viên cần xóa.

            Tìm kiếm HashNode cần xóa:

                Truy cập hashTable[index].

                Duyệt qua danh sách liên kết (nếu có).

                Sử dụng hai con trỏ: current (để duyệt) và prev (để trỏ đến nút trước current).

            Xóa HashNode:

                Trường hợp 1: Không tìm thấy maSV: Không làm gì cả.

                Trường hợp 2: Tìm thấy maSV:

                    Nếu prev == nullptr (xóa nút đầu tiên): hashTable[index] = current->next.

                    Nếu prev != nullptr: prev->next = current->next.

                    Giải phóng bộ nhớ của current->data (đối tượng SinhVien).

                    Giải phóng bộ nhớ của current.

        Ví dụ minh họa:

            Xóa sinh viên có maSV = "SV000002":

                Tính index = 528.

                Duyệt danh sách liên kết tại hashTable[528].

                Tìm thấy HashNode có maSV = "SV000002".

                prev là nullptr (xóa nút đầu tiên).

                hashTable[528] = hashTable[528]->next (trỏ đến HashNode("SV123456", data123456)).

                Giải phóng bộ nhớ của HashNode("SV000002", data2).

            Bảng băm sau khi xóa:

                hashTable[100] -> HashNode("SV000001", data1) -> nullptr

                hashTable[528] -> HashNode("SV123456", data123456) -> nullptr

                hashTable[99999] -> HashNode("SV999999", data999999) -> nullptr

## Phần 6: Hoạt động chi tiết của Cây AVL

### 1. Thêm sinh viên vào cây AVL (insertAVL)

        Khi thêm một sinh viên, hàm insertAVL thực hiện các bước sau:

        Thêm như cây nhị phân tìm kiếm thông thường:

        So sánh maSV của sinh viên mới với maSV tại nút gốc.

        Nếu nhỏ hơn, duyệt đệ quy sang cây con trái.

        Nếu lớn hơn, duyệt đệ quy sang cây con phải.

        Khi đến nút lá (nullptr), tạo nút mới chứa thông tin sinh viên và gắn vào vị trí đó.

        Cập nhật chiều cao (height):

        Sau khi thêm, cập nhật chiều cao của các nút tổ tiên của nút mới thêm, bắt đầu từ cha của nút mới lên đến gốc.

        height của một nút bằng max(height(cây con trái), height(cây con phải)) + 1.

        Kiểm tra và cân bằng lại cây:

        Tính hệ số cân bằng (balance factor) của các nút tổ tiên, bắt đầu từ cha của nút mới lên đến gốc.

        balance factor = height(cây con trái) - height(cây con phải).

        Nếu balance factor của một nút > 1 hoặc < -1, cây mất cân bằng tại nút đó.

        Thực hiện các phép xoay để cân bằng lại cây, tùy thuộc vào từng trường hợp:

        Các trường hợp mất cân bằng và cách xoay:

        Left Left (LL):

        Mất cân bằng do thêm vào cây con trái của cây con trái.

        Cách xoay: Xoay phải tại nút mất cân bằng.

        Ví dụ:

        z (2)                       y (0)
        /                           /   \
        y (1)        =>             x (0)  z (0)
        /
        x (0)
        content_copy
        Use code with caution.

        Right Right (RR):

        Mất cân bằng do thêm vào cây con phải của cây con phải.

        Cách xoay: Xoay trái tại nút mất cân bằng.

        Ví dụ:

        z (-2)                      y (0)
        \                          /   \
            y (-1)      =>            z (0)  x (0)
            \
            x (0)
        content_copy
        Use code with caution.

        Left Right (LR):

        Mất cân bằng do thêm vào cây con phải của cây con trái.

        Cách xoay: Xoay trái tại nút con trái của nút mất cân bằng, sau đó xoay phải tại nút mất cân bằng.

        Ví dụ:

        z (2)                       z (2)                     x (0)
        /                           /                         /   \
        y (1)        =>             x (1)        =>           y (0)  z (0)
        \                         /
            x (0)                    y (0)
        content_copy
        Use code with caution.

        Right Left (RL):

        Mất cân bằng do thêm vào cây con trái của cây con phải.

        Cách xoay: Xoay phải tại nút con phải của nút mất cân bằng, sau đó xoay trái tại nút mất cân bằng.

        Ví dụ:

        z (-2)                      z (-2)                        x (0)
        \                           \                          /   \
            y (-1)     =>              x (-1)       =>           z (0)  y (0)
        /                             \
        x (0)                           y (0)
        content_copy
        Use code with caution.

### 2. Xóa sinh viên khỏi cây AVL (deleteAVL)

        Khi xóa một sinh viên, hàm deleteAVL thực hiện các bước sau:

        Xóa như cây nhị phân tìm kiếm thông thường:

        Tìm nút cần xóa dựa vào maSV.

        Trường hợp 1: Nút cần xóa là nút lá: Xóa trực tiếp.

        Trường hợp 2: Nút cần xóa có 1 con: Thay thế nút cần xóa bằng nút con của nó.

        Trường hợp 3: Nút cần xóa có 2 con:

        Tìm nút có giá trị nhỏ nhất trong cây con phải (hoặc lớn nhất trong cây con trái) - gọi là nút thay thế.

        Thay thế giá trị của nút cần xóa bằng giá trị của nút thay thế.

        Xóa nút thay thế (bây giờ nút thay thế chỉ có thể là nút lá hoặc có 1 con - quay lại trường hợp 1 hoặc 2).

        Cập nhật chiều cao: Cập nhật chiều cao của các nút tổ tiên của nút bị xóa (hoặc nút thay thế bị xóa).

        Kiểm tra và cân bằng lại cây:

        Tính hệ số cân bằng của các nút tổ tiên, bắt đầu từ cha của nút bị xóa (hoặc nút thay thế bị xóa) lên đến gốc.

        Nếu mất cân bằng, thực hiện các phép xoay tương tự như khi thêm để cân bằng lại cây. Các trường hợp mất cân bằng và cách xoay giống hệt như khi thêm (LL, RR, LR, RL).

        Ví dụ Xóa và cân bằng cây:
        Giả sử ta có cây AVL sau, và cần xóa nút có mã sinh viên là 30:

        40(1)
            /       \
            20(0)      60(-1)
            /   \        \
        10(0) 30(0)    70(0)
        content_copy
        Use code with caution.

        Tìm và xóa nút 30: Nút 30 là nút lá, xóa trực tiếp.

        40(1)
            /       \
            20(-1)     60(-1)
            /            \
        10(0)         70(0)
        content_copy
        Use code with caution.

        Cập nhật chiều cao: Cập nhật chiều cao của nút 20, 40.

        Kiểm tra cân bằng: Nút 20 có balance factor = -1 (vẫn cân bằng). Nút 40 có balance factor = 1 (vẫn cân bằng).

        Ví dụ khác: Xóa nút có mã sinh viên 10

        Tìm và xóa nút 10

        40(2)
            /       \
            20(1)     60(-1)
            /            \
        10(0)         70(0)

                40(2)
            /       \
            20(0)     60(-1)
                        \
                        70(0)
        content_copy
        Use code with caution.

        Cập nhật chiều cao các nút 20, 40

        Kiểm tra cân bằng: Nút 20 có balance factor = 0 (vẫn cân bằng). Nút 40 có balance factor = 2 -> Mất cân bằng. Nút con trái của 40 là 20 có balance factor = 0 -> Trường hợp Left Left (LL), xoay phải tại nút 40.

        20(0)
            /       \
            10(0)     40(-1)
                        \
                        60(-1)
                        \
                        70(0)

                20(0)
            /       \
                    40(0)
                    /    \
                60(0) 70(0)
        content_copy
        Use code with caution.
