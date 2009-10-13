#include <iostream>

#include "../libmoor/LibMoor.h"

int main() {
    CLibMoor * Instance;
    std::string hash;
    hash = "<<ahMM5fGND5/lZDHA0iGxJUZd1ca0RHDZNa49clQyHvK9/SwFsi7dsaYVn/XUyTtp/TVJzjfWXq7cczWWTpaJiptfcnGUpPA579UK4Jr/JNm-XEFXSa7ltA6Y8WafSk41E8claZw0c8QTYGMJpHcWrKmfK09sfyJdNa5f8Z8ZzV4kCrAV XFwRVKtIxIGk0ywy5R7BxYviVUN5qjuHFJq6nUR-lup4lSKxXxLMTRmmPJ6a5v6DdMBws3GrbDo8KQDT7vMk8BATVjMcputVtEf0H7KIgdtdqusQ5BeVCL3TPnO795DN wGCqDGQR9XyOu/IOw2BDsirSLV0oa6LiCVwJ76OXXNDGQggKNyLQbS1KOvNHAm9a k/F17tA8mrrRKPU80nJ5G4MBoCtsnpVb-8DouhWLq-/4P4QYUkjW1oOBQkg7tk0XpDfF4aNkbymNt4SNrI0pJmL65IF54p4qvJnYcKdjAoswZOFDz3UQTC3jNs8K/iPb4VGzh-7VgUpP01N0VFhR8qKSxFew7xq2-cuD-w2rKnlRRaxuB0SO64dUWl6v8nmSLpBX5qJuqvcrYi95GgOCM2J7KlHesf/7SbG3V9FNN5oSFxilfkOKbesA4o4GtFaDA7mIPl3PhU6kfDn/UN-kDuMWu8w6ihO9djvmGt4K3YBreMlhyBAyLRDfy-IEhasMUgwOAjAWzdX88lPZ/zDPEVgHdUEWyRQQFkUVtG0Q==>>";
    Instance = new CLibMoor();

	if (Instance -> Dehash(hash) != 0) {
		std::cout << "Podany hashcode jest nieprawidlowy..." << std::endl;
		return 1;
	}
	else
		std::cout << "Podany hashcode jest prawidlowy... Rozpoczytam pobieranie listy segmentow..." << std::endl;

	return 0;
}
