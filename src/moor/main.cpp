#include <iostream>

#include "../libmoor/LibMoor.h"

int main() {
    CLibMoor * Instance;
    std::string hash;
    hash = "<<ahQbTU4m9TnE2dW4KPF4gEV-4m3AcYUnRtm40K15lfZcPBpamtY4kNt7HtKjzi\
S7tpfiKGtmlXw4wv15Thi-PphTUFtsKJgzWjzWUgmcCCj4QX4Q7ItbkjRxnE0bAQ\
E7n1dF6T7WVsPifR/FNFIByqrCwYPuCp7aRT4wpKkcHoS3xHg3AW3Gg1od/ZBD5x\
ad-7j4UARF8STlDsxaHYq0NuDwYh5crP88CwnqadIs-fEib6n7XYvTE0rsdQA16e\
kKAPZ801YNbrfd8H6ZHQjFwA0lC4BBpjHlrPnN8lw9azgbwjy/M9OBhRBREmU5CZ\
kjToy0xHUQUc3OM5Q7RQWyPpezNrctxEvD4-go0aVL50wUXXlI9yp31TbcLCOz2a\
bjON8JtghB/idGaBLcDLOeEyBFzmHriE6UrJTU9UUdVVIXB151p6sW5dl-7hLpbx\
aaB6jcoqp8F6qVVqMBgvNyD7hwj6Wan-qPKVoymYQVzjRZzFqmIswYt6itrHk-o0\
tirj8QSk19kh8Wh0mkRMWQPIj5SdAN8P9NTFpUqKEp1OE67eoe vS-fH9sZAztk3u\
G8vV4ldreOVnOIiJHE6FzKzeyg==>>";
    Instance = new CLibMoor();

	if (Instance -> Dehash(hash) != 0) {
		std::cout << "Podany hashcode jest nieprawidlowy..." << std::endl;
		return 1;
	}
	else
		std::cout << "Podany hashcode jest prawidlowy... Rozpoczytam pobieranie listy segmentow..." << std::endl;

	return 0;
}
