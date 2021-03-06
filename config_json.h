const char *json = "\
{\
	\"ConfigID\": \"TEMP_MODULE-1\",\
	\"Version\": \"1.1\",\
	\"DisplayTemperatureAs\": \"Celsius\",\
	\"Products\": [{\
		\"ProductID\": \"TEMP_MODULE\",\
\
		\"OperationalMode\": \"ContinuousConversion\",\
		\"PowerMode\": \"Low\",\
		\"ContinuousReadEnable\": \"False\",\
		\"AVDD\": \"3.3\",\
		\"DVDD\": \"3.3\",\
		\"HardwareParameters\": [{\
			\"MeasurementID\": \"0\",\
			\"MeasurementType\": \"Temperature\",\
			\"SensorType\": \"Thermocouple.T-Type\",\
			\"TemperatureMin\": \"-200\",\
			\"TemperatureMax\": \"400\",\
			\"Gain\": \"64\",\
			\"VBiasEnable\": \"False\",\
			\"FilterType\": \"FIR.25SPS\",\
			\"FS\": \"384\"\
		}, {\
			\"MeasurementID\": \"1\",\
			\"MeasurementType\": \"Temperature\",\
			\"SensorType\": \"RTD3Wire.PT100\",\
			\"Gain\": \"32\",\
			\"ExcitationCurrent\": \"0.000500\",\
			\"ReferenceResistor\": \"5110\",\
			\"FilterType\": \"FIR.25SPS\",\
			\"FS\": \"384\"\
		}],\
		\"Results\": {\
			\"50Hz60HzRejectionSelected\": \"True\",\
			\"TotalMeasurementTime\": \"0.084\"\
		},\
		\"VirtualEvalParameters\": {\
			\"FilterOption\": \"BestFit\",\
			\"DisplayMode\": \"Basic\"\
		},\
		\"SimulationParameters\": [{\
			\"MeasurementID\": \"0\",\
			\"Temperature\": \"25\"\
		}, {\
			\"MeasurementID\": \"1\",\
			\"Temperature\": \"0\"\
		}]\
	}]\
}\
\
";