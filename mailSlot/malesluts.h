HANDLE CreateMaleSlut(LPSTR name) {
	HANDLE mail;
	mail = CreateMailslot(name, 0, MAILSLOT_WAIT_FOREVER, NULL);
	if (mail == INVALID_HANDLE_VALUE) {
		printf("Error creating Maleslut\n");
	}
	return mail;
}

HANDLE ConnectMaleSlut(LPSTR name) {
	HANDLE mail;
	do
	{
			/* Open Mailslot */
		mail = CreateFile(
			name,
			GENERIC_WRITE, 
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING, 
			FILE_ATTRIBUTE_NORMAL,
			NULL);
	} while ( mail == INVALID_HANDLE_VALUE);
	// if (mail == INVALID_HANDLE_VALUE) {
	// 	printf("Error connecting to Maleslut...\n");
	// }
	return mail;
}