using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace geovis.scripts
{
    // GEOVIS CSV metrics' indexes.
    public enum CsvIdx
    {
        TimeMilliseconds,
        Heading,
        Pitch,
        Roll,
        LinearAccelX,
        LinearAccelY,
        LinearAccelZ,
        LinearAccelMagnitude,
        GravityAccelX,
        GravityAccelY,
        GravityAccelZ,
        GpsAltitudeIsValid,
        GpsAltitudeIsUpdated,
        GpsAltitudeAge,
        GpsAltitudeMeters,
        GpsCourseIsValid,
        GpsCourseIsUpdated,
        GpsCourseAge,
        GpsCourseDeg,
        GpsDateIsValid,
        GpsDateIsUpdated,
        GpsDateAge,
        GpsTimeIsValid,
        GpsTimeIsUpdated,
        GpsTimeAge,
        Iso8601,
        HdopIsValid,
        HdopIsUpdated,
        HdopAge,
        HdopValue,
        LocationIsVaLid,
        LocationIsUpdated,
        LocationAge,
        LocationLat,
        LocationLng,
        LocationRawLatBillionths,
        LocationRawLngBillionths,
        SatellitesTrackingIsValid,
        SatellitesTrackingIsUpdated,
        SatellitesTrackingAge,
        GpsSatellitesTrackingValue,
        GpsSpeedIsValid,
        GpsSpeedIsUpdated,
        GpsSpeedAge,
        GpsSpeedKnots,
        GpsCharsProcessed,
        GpsSentencesWithFix,
        GpsFailedChecksums,
        GpsPassedChecksums,
        AmbientTemperature,
        AmbientPressure,
        RelativeHumidity,
        PressureAltitude,
        Phase,
    }

    public struct VehicleSimulationState
    {
        public double roll;
        public double pitch;
        public double heading;
        public double pressure_altitude;
        public double temp;
        public double delta;
        public string iso8601;
        public double agl;
        public double acceleration;
        public double latitude;
        public double longitude;
        public double speed;
        public double gps_altitude;
        public int phase;
        public int apogee_cause;
        public double main_alt;
    }

    class Program
    {
        const double PAD_ALTITUDE_METERS = 287.5;
        const double MAIN_ALTITUDE = 0.0;
        const int APOGEE_CAUSE = 4;
        const int NUM_SAMPLES = 2414;
        const double LATITUDE_INITIAL = 35.487229;
        const double LATITUDE_FINAL = 35.481661;
        const double LONGITUDE_INITIAL = -85.95816;
        const double LONGITUDE_FINAL = -85.957343;

        
        static double GetLinearTransitionBetweenTwoPoints(double initial_value, double final_value, int num_samples, int current_position)
        {
            return (final_value - initial_value) / num_samples * current_position + initial_value;
        }

        static double GetLinearLatitudeTransition(int current_position)
        {
            return GetLinearTransitionBetweenTwoPoints(LATITUDE_INITIAL, LATITUDE_FINAL, NUM_SAMPLES, current_position);
        }

        static double GetLinearLongitudeTransition(int current_position)
        {
            return GetLinearTransitionBetweenTwoPoints(LONGITUDE_INITIAL, LONGITUDE_FINAL, NUM_SAMPLES, current_position);
        }

        static void ExpressFailure(string message)
        {
            Console.WriteLine("fatal error: " + message);
        }

        static void Main(string[] args)
        {
            // Validate args.

            if (args.Length < 2)
            {
                ExpressFailure("must specify an [input path] and [output directory].");
                return;
            }
            if (args.Length > 2) Console.WriteLine("Ignoring arguments after output directory path \"" + args[1] + "\".");

            if (!File.Exists(args[0]))
            {
                ExpressFailure("no file found at input path \"" + args[0] + "\".");
                return;
            }

            var output_directory =  args[1];
            if (!Directory.Exists(output_directory))
            {
                ExpressFailure("no directory found at path \"" + args[1] + "\".");
                return;
            }

            // Once validated, process flight data at input path.
            var file_name = DateTime.Now.ToString("dd-MM-yyyy_hh_mm_ss") + ".json";
            var output_file_path = Path.Combine(output_directory, file_name);
            ProcessFlight(args[0], output_file_path);
        }

        // Open the flight log and read every csv line.
        static void ProcessFlight(string input_path, string output_path)
        {
            var reader = new StreamReader(input_path);
            var writer = new StreamWriter(File.OpenWrite(output_path));
            TimeSpan system_startup_time;
            var time_since_startup = TimeSpan.Zero;


            Func<string, bool> is_flight_data = input_csv_line =>
            {
                if (input_csv_line == null) return false;

                // If number or leading decimal floating-point, true.
                if (Char.IsDigit(input_csv_line.ElementAt(0))
                    || input_csv_line.ElementAt(0) == '.') return true;

                return false;
            };

            // Read past the header (non-csv) line.
            string line;
            do
            {
                line = reader.ReadLine(); 
            } while (!is_flight_data(line));


            system_startup_time = TimeSpan.FromMilliseconds(Double.Parse(
                line.Split(',')[(int)CsvIdx.TimeMilliseconds]
            ));
            for (int position = 0; line != null; line = reader.ReadLine())
            {
                ++position;

                // Get the line's values.
                var vals = line.TrimEnd(',') // disallow trailing ','
                               .Split(',');  // and get each delimited value.

                { // Calculate flight-time delta.
                    time_since_startup = TimeSpan.FromMilliseconds(Double.Parse(vals[(int)CsvIdx.TimeMilliseconds])) - system_startup_time;
                }

                var pressure_altitude = Double.Parse(vals[(int)CsvIdx.PressureAltitude]);
                
                var state = new VehicleSimulationState
                {
                    delta =             Math.Round(time_since_startup.TotalSeconds),
                    pressure_altitude = pressure_altitude,
                    agl =               Math.Round(pressure_altitude - PAD_ALTITUDE_METERS, 1),
                    apogee_cause =      APOGEE_CAUSE,
                    main_alt =          MAIN_ALTITUDE,
                    heading =           Double.Parse(vals[(int)CsvIdx.Heading]),
                    pitch =             Double.Parse(vals[(int)CsvIdx.Pitch]),
                    roll =              Double.Parse(vals[(int)CsvIdx.Roll]),
                    acceleration =      Math.Round(Double.Parse(vals[(int)CsvIdx.LinearAccelMagnitude]) / 9.80665, 4), // get g's from m*s^-2
                    temp =              Double.Parse(vals[(int)CsvIdx.AmbientTemperature]),
                    latitude =          Math.Round(GetLinearLatitudeTransition(position), 7),
                    longitude =         Math.Round(GetLinearLongitudeTransition(position), 7),
                    iso8601 =           vals[(int)CsvIdx.Iso8601],
                    gps_altitude =      Double.Parse(vals[(int)CsvIdx.GpsAltitudeMeters]),
                    speed =             Double.Parse(vals[(int)CsvIdx.GpsSpeedKnots]),
                    phase =             int.Parse(vals[(int)CsvIdx.Phase]),
                };

                {
                    var json_line = JsonConvert.SerializeObject(state);
                    writer.WriteLine(json_line);
                }
            }

            reader.Close();
        }
    }
}
