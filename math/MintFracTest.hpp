void fracmintTest() {
	uint_fast64_t seed = 202212022100123456;
	mt19937_64 engine(seed);
	long long x, y, z, w;
	{
		x = engine();
		y = engine();
		fracmint fm0(x, y);
		fracmint fm1((mint)x, y);
		fracmint fm2(x, (mint)y);
		fracmint fm3((mint)x, (mint)y);
		fracmint fm4 = fm0;
		fracmint fm5 = fm3.val();
		// constructor
		if (fm0.val() == fm0.val()) cout << "constructor : Passed" << endl;
		else  cout << "constructor : NotPassed" << endl;
		// constructor
		if (fm0.val() == fm1.val()) cout << "constructor : Passed" << endl;
		else  cout << "constructor : NotPassed" << endl;
		// constructor
		if (fm0.val() == fm2.val()) cout << "constructor : Passed" << endl;
		else  cout << "constructor : NotPassed" << endl;
		// constructor
		if (fm0.val() == fm3.val()) cout << "constructor : Passed" << endl;
		else  cout << "constructor : NotPassed" << endl;
		// =
		if (fm0.val() == fm4.val()) cout << "= : Passed" << endl;
		else  cout << "constructor : NotPassed" << endl;
		// =
		if (fm0.val() == fm5.val()) cout << "= : Passed" << endl;
		else  cout << "constructor : NotPassed" << endl;
	}
	{
		x = engine();
		y = engine();
		mint m = x;
		m /= y;
		fracmint fm(x, y);
		// val
		if (m.val() == fm.val().val()) cout << "val() : Passed" << endl;
		else  cout << "val() : NotPassed" << endl;
		// ==
		if (m == fm) cout << "== : Passed" << endl;
		else  cout << "== : NotPassed" << endl;
		// !=
		if (!(m != fm)) cout << "!= : Passed" << endl;
		else  cout << "!= : NotPassed" << endl;
		long long n = m.val();
		// ==
		if (n == fm) cout << "== : Passed" << endl;
		else  cout << "== : NotPassed" << endl;
		// !=
		if (!(n != fm)) cout << "!= : Passed" << endl;
		else  cout << "!= : NotPassed" << endl;
	}
	{
		x = engine();
		y = engine();
		z = engine();
		w = engine();
		mint m0 = x;
		m0 /= y;
		fracmint fm0(x, y);
		mint m1 = z;
		m1 /= w;
		fracmint fm1(z, w);
		{// + 
			mint m2 = m0 + m1;
			fracmint fm2 = fm0 + fm1;
			if (m2 == fm2) cout << "+ : Passed" << endl;
			else  cout << "+ : NotPassed" << endl;
		}
		{// - 
			mint m2 = m0 - m1;
			fracmint fm2 = fm0 - fm1;
			if (m2 == fm2) cout << "- : Passed" << endl;
			else  cout << "- : NotPassed" << endl;
		}
		{// * 
			mint m2 = m0 * m1;
			fracmint fm2 = fm0 * fm1;
			if (m2 == fm2) cout << "* : Passed" << endl;
			else  cout << "* : NotPassed" << endl;
		}
		{// / 
			mint m2 = m0 / m1;
			fracmint fm2 = fm0 / fm1;
			if (m2 == fm2) cout << "/ : Passed" << endl;
			else  cout << "/ : NotPassed" << endl;
		}

		{// + 
			mint m2 = m0 + m1;
			fracmint fm2 = fm0 + m1.val();
			if (m2 == fm2) cout << "+ : Passed" << endl;
			else  cout << "+ : NotPassed" << endl;
		}
		{// - 
			mint m2 = m0 - m1;
			fracmint fm2 = fm0 - m1.val();
			if (m2 == fm2) cout << "- : Passed" << endl;
			else  cout << "- : NotPassed" << endl;
		}
		{// * 
			mint m2 = m0 * m1;
			fracmint fm2 = fm0 * m1.val();
			if (m2 == fm2) cout << "* : Passed" << endl;
			else  cout << "* : NotPassed" << endl;
		}
		{// / 
			mint m2 = m0 / m1;
			fracmint fm2 = fm0 / m1.val();
			if (m2 == fm2) cout << "/ : Passed" << endl;
			else  cout << "/ : NotPassed" << endl;
		}

		{// + 
			mint m2 = m0 + m1;
			fracmint fm2 = fm0 + m1;
			if (m2 == fm2) cout << "+ : Passed" << endl;
			else  cout << "+ : NotPassed" << endl;
		}
		{// - 
			mint m2 = m0 - m1;
			fracmint fm2 = fm0 - m1;
			if (m2 == fm2) cout << "- : Passed" << endl;
			else  cout << "- : NotPassed" << endl;
		}
		{// * 
			mint m2 = m0 * m1;
			fracmint fm2 = fm0 * m1;
			if (m2 == fm2) cout << "* : Passed" << endl;
			else  cout << "* : NotPassed" << endl;
		}
		{// / 
			mint m2 = m0 / m1;
			fracmint fm2 = fm0 / m1;
			if (m2 == fm2) cout << "/ : Passed" << endl;
			else  cout << "/ : NotPassed" << endl;
		}
	}
	{
		x = engine();
		y = engine();
		z = engine();
		w = engine();
		mint m0 = x;
		m0 /= y;
		fracmint fm0(x, y);
		mint m1 = z;
		m1 /= w;
		fracmint fm1(z, w);
		{// ++
			m0++;
			fm0++;
			if (m0 == fm0) cout << "++ : Passed" << endl;
			else  cout << "++ : NotPassed" << endl;
		}
		{// --
			m1--;
			fm1--;
			if (m1 == fm1) cout << "-- : Passed" << endl;
			else  cout << "-- : NotPassed" << endl;
		}
	}
	{
		x = engine();
		y = engine();
		z = engine();
		w = engine();
		mint m0 = x;
		m0 /= y;
		fracmint fm0(x, y);
		mint m1 = z;
		m1 /= w;
		fracmint fm1(z, w);
		{// += 
			mint m2 = m1;
			m2 += m0;
			fracmint fm2 = fm1;
			fm2 += fm0;
			if (m2 == fm2) cout << "+= : Passed" << endl;
			else  cout << "+= : NotPassed" << endl;
		}
		{// -= 
			mint m2 = m1;
			m2 -= m0;
			fracmint fm2 = fm1;
			fm2 -= fm0;
			if (m2 == fm2) cout << "-= : Passed" << endl;
			else  cout << "-= : NotPassed" << endl;
		}
		{// *=
			mint m2 = m1;
			m2 *= m0;
			fracmint fm2 = fm1;
			fm2 *= fm0;
			if (m2 == fm2) cout << "*= : Passed" << endl;
			else  cout << "*= : NotPassed" << endl;
		}
		{// /= 
			mint m2 = m1;
			m2 /= m0;
			fracmint fm2 = fm1;
			fm2 /= fm0;
			if (m2 == fm2) cout << "/= : Passed" << endl;
			else  cout << "/= : NotPassed" << endl;
		}
	}
	{
		x = engine();
		y = engine();
		z = engine();
		w = engine();
		mint m0 = x;
		m0 /= y;
		fracmint fm0(x, y);
		mint m1 = z;
		m1 /= w;
		fracmint fm1(z, w);
		{// += 
			mint m2 = m1;
			m2 += m0;
			fracmint fm2 = fm1.val();
			fm2 += fm0;
			if (m2 == fm2) cout << "+= : Passed" << endl;
			else  cout << "+= : NotPassed" << endl;
		}
		{// -= 
			mint m2 = m1;
			m2 -= m0;
			fracmint fm2 = fm1.val();
			fm2 -= fm0;
			if (m2 == fm2) cout << "-= : Passed" << endl;
			else  cout << "-= : NotPassed" << endl;
		}
		{// *=
			mint m2 = m1;
			m2 *= m0;
			fracmint fm2 = fm1.val();
			fm2 *= fm0;
			if (m2 == fm2) cout << "*= : Passed" << endl;
			else  cout << "*= : NotPassed" << endl;
		}
		{// /= 
			mint m2 = m1;
			m2 /= m0;
			fracmint fm2 = fm1.val();
			fm2 /= fm0;
			if (m2 == fm2) cout << "/= : Passed" << endl;
			else  cout << "/= : NotPassed" << endl;
		}
	}
	{
		x = engine();
		y = engine();
		z = engine();
		w = engine();
		mint m0 = x;
		m0 /= y;
		fracmint fm0(x, y);
		mint m1 = z;
		m1 /= w;
		fracmint fm1(z, w);
		{// += 
			mint m2 = m1;
			m2 += m0;
			fracmint fm2 = fm1.val().val();
			fm2 += fm0;
			if (m2 == fm2) cout << "+= : Passed" << endl;
			else  cout << "+= : NotPassed" << endl;
		}
		{// -= 
			mint m2 = m1;
			m2 -= m0;
			fracmint fm2 = fm1.val().val();
			fm2 -= fm0;
			if (m2 == fm2) cout << "-= : Passed" << endl;
			else  cout << "-= : NotPassed" << endl;
		}
		{// *=
			mint m2 = m1;
			m2 *= m0;
			fracmint fm2 = fm1.val().val();
			fm2 *= fm0;
			if (m2 == fm2) cout << "*= : Passed" << endl;
			else  cout << "*= : NotPassed" << endl;
		}
		{// /= 
			mint m2 = m1;
			m2 /= m0;
			fracmint fm2 = fm1.val().val();
			fm2 /= fm0;
			if (m2 == fm2) cout << "/= : Passed" << endl;
			else  cout << "/= : NotPassed" << endl;
		}
	}
}