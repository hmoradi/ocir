// Model Setting 
#define GRIDEYE_WIDTH 128
#define GRIDEYE_HEIGHT 128
#define MLX_WIDTH 256
#define MLX_HEIGHT 64
#define GRIDEYE_RAW_ROWS 8
#define GRIDEYE_RAW_COLS 8
#define MLX_RAW_ROWS 4
#define MLX_RAW_COLS 16
#define BACKGROUND_FRAMES 250
#define MIN_BODY_TEMP 60
#define MIN_BODY_DIFF 1
#define TRAJECTORY_LENGTH 100000
#define DETECTION_DELAY 3
#define MIN_BODY_AREA 20
#define MAX_BODY_AREA 60
#define PacketLength 134
#define SENSOR 0			//0 is for GridEye, 1 is for MLX
#define HISTORY_LENGTH 10000
#define WINDOW_SIZE 10
// Mounting Setting
#define OPOSITE_DIRECTION false
#define VIEW 0
// Debug Setting
#define PRINT_DEBUG_MSG true 
#define PRINT_MAX_TEMP false
#define LIVE false
#define DEBUG_LEVEL 0
#define SAVE_VIDEO false
#define SAVE_IMAGE true	
#define INPUT_FILE "../data/dataexp10.txt"
#define START_FRAME 1
#define END_FRAME 20000
// XMPP Setting
#define enable_publishing_indicator_count false
#define ENABLE_ACTUATION false
#define reset_count_at_midnight false
#define ENABLE_XMPP_REPORTING false
// Visualization Setting
#define SHOW_IMAGE true 
#define SHOW_DIFF true
#define SHOW_HIST true
#define WAIT_TIME 10
// Blob Detection
#define BLOB_DETECTION false
#define blobMinThreshold 12
#define blobMaxThreshold 25
#define blobThresholdStep 1
#define minDistBetweenBlobs 4
#define blobFilterByArea true
#define blobMinArea 2
#define blobMaxArea 5000
#define blobFilterByCircularity false
#define blobMinCircularity 0.1
#define blobMaxCircularity 1
#define blobFilterByConvexity false
#define blobMinConvexity 0.87
#define blobMaxConvexity 1
#define blobFilterByInertia false
#define blobMinInertiaRatio 0.01
#define blobMaxInertiaRatio 1
#define blobFilterByColor false
#define blobColor 255