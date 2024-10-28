#pragma once

class hex {
	private:
		int _q;
		int _r;
		int _s;

	public:
		int get_q();
		int get_r();
		int get_s();
		void draw();
		hex(int q, int r, int s);
};
