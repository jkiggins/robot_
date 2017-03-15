void debug_line_sensor(int mode)
{
	int pos = read_line();
	Serial.print("D: ");Serial.print(density);
	Serial.print("  POS: ");Serial.print(pos);
	
	if(mode == 0)
	{
		Serial.print("  Analog Values: ");
		print_arri(svals, NUMLSENSORS);
	}
	else
	{
		Serial.print("    Digital Values: ");
		print_char_bitwise(sv_char);
	}

	Serial.println();
}

void debug_dips()
{
	Serial.print(digitalRead(29));
	Serial.print(digitalRead(30));
	Serial.print(digitalRead(31));
	Serial.print(digitalRead(33));
	Serial.println();
}

void debug_calibration()
{
	Serial.print("HIGH: ");
	print_arri(high, NUMLSENSORS);

	Serial.println();

	Serial.print("LOW: ");
	print_arri(low, NUMLSENSORS);
}