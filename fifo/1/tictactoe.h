void print_game(int m[3][3]) {
	int i,j;
	printf("The current game :\n");
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++)
			printf("%d ",m[i][j]);
		printf("\n");
	}
}

void mark(int m[3][3], int row, int column, int choice) {
	m[row-1][column-1] = choice;
}

void start_game(int m[3][3]) {
	int i,j;
	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			m[i][j] = 0;
}

int row(int m[3][3]) {
	int i;
	for (i = 0; i < 3; i++) {
		if (m[i][0] == m[i][1] && m[i][1] == m[i][2] )	
			if (m[i][0] != 0) return m[i][0];
				// else return m[i][0];
	}
	return 0;
}

int column(int m[3][3]) {
	int i;
	for (i = 0; i < 3; i++) {
		if (m[0][i] == m[1][i] && m[1][i] == m[2][i] )
			if (m[0][i] != 0) return m[0][i];
				// else return m[0][i];
	}
	return 0;
}

int diagonal(int m[3][3]) {
	if (m[0][0] == m[1][1] && m[1][1] == m[2][2] )	
		if (m[1][01] != 0) return m[1][01];
			// else return 2;
	if (m[0][2] == m[1][1] && m[1][1] == m[2][0] )	
		if (m[1][1] != 0) return m[1][1];
			// else return 2;
	return 0;
}

int win(int m[3][3]) {
	int w = 0;
	w = row(m);
	if (w == 0) {
		w = column(m);
		if (w == 0) w = diagonal(m);
	}
	if (w) return w;
	int i,j;
	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			if (m[i][j] == 0) return 0;
	return 3;
}
