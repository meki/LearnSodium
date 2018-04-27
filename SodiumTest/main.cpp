#include <iostream>
#include <sodium/sodium.h>


int main()
{
	using namespace std;
	using namespace sodium;

	stream_sink<int> s1;

	stream_sink<int> s2;
	
	auto s11 = s1.map([](int i) { return i + 1; });
	auto s22 = s2.map([](int i) { return i + 100; });

	auto s12 = s11.or_else(s22);

	auto m = s11.merge(s22, [](int i, int j)
	{
		return i + j;
	});

	auto c = s12.hold(0);
	auto c2 = m.hold(0);

	auto s = [](auto l, auto r) {
		return l + r;
	};
	
	auto unlisten1 = c.listen([](int v) {cout << "listen1: value = " << v << endl; });
	auto unlisten2 = c2.listen([](int i) { cout << "listen2: sum = " << i << endl; });
	
	
	s1.listen([](int i) { cout << "s1 send " << i << endl; });
	s2.listen([](int i) { cout << "s2 send " << i << endl; });

	s1.send(10);

	s2.send(100);

	s1.send(12);

	s2.send(111);

	cout << "unlisten()" << endl;

	unlisten1();

	s1.send(5);
	s2.send(7);


	cout << "info: c2 = " << c.sample() << endl;

    return 0;
}

