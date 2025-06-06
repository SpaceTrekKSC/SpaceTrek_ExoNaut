#include "ExoNaut_AICam.h"

void ExoNaut_AICam::begin(void)
{
    this->wire = Wire;
    // Wire.begin() is already called in the main sketch
    // with specific pins for ESP32
}

int ExoNaut_AICam::readFromAddr(uint16_t addr, uint8_t *buf, uint16_t leng)
{
    int len = 0;
    Wire.beginTransmission(CAM_DEFAULT_I2C_ADDRESS);
    Wire.write(byte(addr & 0x00FFu));
    Wire.write(byte((addr >> 8) & 0x00FFu));
    Wire.endTransmission();

    uint16_t ts = leng >> 5;
    uint16_t ls = leng - (ts << 5);
    for (int i = 0; i < ts; ++i)
    {
        Wire.requestFrom(CAM_DEFAULT_I2C_ADDRESS, 32, true);
        while (Wire.available())
        {
            *buf++ = Wire.read();
            ++len;
        }
    }
    if (ls > 0)
    {
        Wire.requestFrom(CAM_DEFAULT_I2C_ADDRESS, ls, true);
        while (Wire.available())
        {
            *buf++ = Wire.read();
            ++len;
        }
    }
    return len;
}

int ExoNaut_AICam::writeToAddr(uint16_t addr, const uint8_t *buf, uint16_t leng)
{
    Wire.beginTransmission(CAM_DEFAULT_I2C_ADDRESS);
    Wire.write(byte(addr & 0x00FFu));
    Wire.write(byte((addr >> 8) & 0x00FFu));
    Wire.write(buf, leng);
    Wire.endTransmission();
    return leng;
}

bool ExoNaut_AICam::firmwareVersion(char *str)
{
    readFromAddr(0x0000, (uint8_t *)str, 16);
    return true;
}

int ExoNaut_AICam::currentFunc(void)
{
    char buf;
    readFromAddr(0x0035, (uint8_t *)&buf, 1);
    this->current = buf;
    return (int)buf;
}

bool ExoNaut_AICam::changeFunc(uint8_t new_func)
{
    uint8_t count = 0;
    writeToAddr(0x0035, &new_func, 1);
    delay(50);
    while (true)
    {
        if (currentFunc() != new_func)
        {
            delay(50);
        }
        else
        {
            return true;
        }
        ++count;
        if (count > 80)
        {
            return false;
        }
    }
}

void ExoNaut_AICam::setLed(bool new_state)
{
    byte buf[3] = {0x30, 0x00, 0x00};
    byte ns_b = new_state ? 1 : 0;
    writeToAddr(0x0030, &ns_b, 1);
}

//Any faces detected?
bool ExoNaut_AICam::anyFaceDetected()
{
    if (current == APPLICATION_FACEDETECT)
    {
        return result_summ[1] > 0 ? true : false;
    }
    return false;
}

//Total number of faces recognized
int ExoNaut_AICam::numOfTotalFaceDetected()
{
    if (current == APPLICATION_FACEDETECT)
    {
        return (int)result_summ[1];
    }
    return 0;
}

bool ExoNaut_AICam::anyLearnedFaceDetected()
{
    if (current == APPLICATION_FACEDETECT)
    {
        return result_summ[2] > 0 ? true : false;
    }
    return false;
}

int ExoNaut_AICam::numOfTotalLearnedFaceDetected()
{
    if (current == APPLICATION_FACEDETECT)
    {
        return (int)result_summ[2];
    }
    return 0;
}

bool ExoNaut_AICam::anyUnlearnedFaceDetected()
{
    if (current == APPLICATION_FACEDETECT)
    {
        return result_summ[3] > 0 ? true : false;
    }
    return false;
}

int ExoNaut_AICam::numOfTotalUnlearnedFaceDetected()
{
    if (current == APPLICATION_FACEDETECT)
    {
        return (int)result_summ[3]; // Fixed: Return value at index 3, not the entire array
    }
    return 0;
}

bool ExoNaut_AICam::faceOfIdDetected(uint8_t id)
{
    if (current == APPLICATION_FACEDETECT)
    {
        for (int i = 4; i < 4 + 29; ++i)
        {
            if (result_summ[i] == id)
            {
                return true;
            }
        }
    }
    return false;
}

//Returns the specific face ID
bool ExoNaut_AICam::getFaceOfId(uint8_t id, WonderCamFaceDetectResult *p)
{
    memset(p, 0, sizeof(WonderCamFaceDetectResult));
    if (current != APPLICATION_FACEDETECT)
    {
        return false;
    }
    for (int i = 4; i < 4 + 29; ++i)
    {
        if (result_summ[i] == id)
        {
            uint16_t index = i - 4;
            index = 0x30 + index * 16;
            readFromAddr(0x0400 + index, (uint8_t *)p, 16);

            return true;
        }
    }
    return false;
}

//Returns the face without ID of the specified sequence number
bool ExoNaut_AICam::getFaceOfIndex(uint8_t index, WonderCamFaceDetectResult *p)
{
    memset(p, 0, sizeof(WonderCamFaceDetectResult));
    if (current != APPLICATION_FACEDETECT)
    {
        return false;
    }
    for (int i = 4; i < 4 + 29; ++i)
    {
        if (result_summ[i] == 0xFF)
        {
            --index;
            if (index == 0)
            {
                uint16_t index_ = i - 4;
                index_ = 0x30 + index_ * 16;
                readFromAddr(0x0400 + index_, (uint8_t *)p, 16);
                return true;
            }
        }
    }
    return false;
}

// Any objects detected？*/
bool ExoNaut_AICam::anyObjDetected()
{
    if (current != APPLICATION_OBJDETECT)
    {
        return false;
    }
    return result_summ[1] > 0 ? true : false;
}

int ExoNaut_AICam::numOfObjDetected()
{
    if (current != APPLICATION_OBJDETECT)
    {
        return 0;
    }
    return (int8_t)(result_summ[1]);
}

bool ExoNaut_AICam::objIdDetected(uint8_t id)
{
    if (current != APPLICATION_OBJDETECT)
    {
        return false;
    }
    for (int i = 2; i < 2 + 29; ++i)
    {
        if (result_summ[i] == id)
        {
            return true;
        }
    }
    return false;
}

int ExoNaut_AICam::numOfObjIdDetected(uint8_t id)
{
    if (current != APPLICATION_OBJDETECT)
    {
        return 0;
    }
    int count = 0;
    for (int i = 2; i < 2 + 29; ++i)
    {
        if (result_summ[i] == id)
        {
            ++count;
        }
    }
    return count;
}

bool ExoNaut_AICam::objDetected(uint8_t id, uint8_t index, WonderCamObjDetectResult *p)
{
    memset(p, 0, sizeof(WonderCamObjDetectResult));
    if (current != APPLICATION_OBJDETECT)
    {
        return false;
    }
    for (int i = 2; i < 2 + 29; ++i)
    {
        if (result_summ[i] == id)
        {
            --index;
        }
        if (index == 0)
        {
            int r = readFromAddr(0x0800 + 48 + ((i - 2) * 16), (uint8_t *)p, 16);
            if (r != 16)
            {
                return false;
            }
            return true;
        }
    }
    return false;
}

int ExoNaut_AICam::classIdOfMaxProb()
{
    if (current != APPLICATION_CLASSIFICATION)
    {
        return 0;
    }
    return (int8_t)result_summ[1];
}

float ExoNaut_AICam::classMaxProb()
{
    uint16_t prob_u16;
    if (current != APPLICATION_CLASSIFICATION)
    {
        return 0;
    }
    memcpy(&prob_u16, &result_summ[2], 2);
    return ((float)((int)(prob_u16))) / 10000.0;
}

float ExoNaut_AICam::classProbOfId(uint8_t id)
{
    uint16_t prob_u16;
    if (current != APPLICATION_CLASSIFICATION)
    {
        return 0;
    }
    memcpy(&prob_u16, &result_summ[16 + (id - 1) * 4], 2);
    return ((float)((int)(prob_u16))) / 10000.0;
}

int ExoNaut_AICam::featureIdOfMaxProb()
{
    if (current != APPLICATION_FEATURELEARNING)
    {
        return 0;
    }
    return (int8_t)result_summ[1];
}

float ExoNaut_AICam::featureMaxProb()
{
    uint16_t prob_u16;
    if (current != APPLICATION_FEATURELEARNING)
    {
        return 0;
    }
    memcpy(&prob_u16, &result_summ[2], 2);
    return ((float)((int)(prob_u16))) / 10000.0;
}

float ExoNaut_AICam::featureProbOfId(uint8_t id)
{
    uint16_t prob_u16;
    if (current != APPLICATION_FEATURELEARNING)
    {
        return 0;
    }
    memcpy(&prob_u16, &result_summ[16 + (id - 1) * 4], 2);
    return ((float)((int)(prob_u16))) / 10000.0;
}

bool ExoNaut_AICam::anyTagDetected(void)
{
    if (current != APPLICATION_APRILTAG)
    {
        return false;
    }
    return result_summ[1] > 0 ? true : false;
}

int ExoNaut_AICam::numOfTotalTagDetected(void)
{
    if (current != APPLICATION_APRILTAG)
    {
        return 0;
    }
    return result_summ[1];
}

bool ExoNaut_AICam::tagIdDetected(uint16_t id)
{
    if (current != APPLICATION_APRILTAG)
    {
        return false;
    }
    int num = result_summ[1];
    for (int i = 2; i < 2 + num; ++i)
    {
        if (result_summ[i] == id)
        {
            return true;
        }
    }
    return false;
}

int ExoNaut_AICam::numOfTagIdDetected(uint16_t id)
{
    int count = 0;
    if (current != APPLICATION_APRILTAG)
    {
        return 0;
    }
    int num = result_summ[1];
    for (int i = 2; i < 2 + num; ++i)
    {
        if (result_summ[i] == id)
        {
            ++count;
        }
    }
    return count;
}

bool ExoNaut_AICam::tagId(uint16_t id, int index, WonderCamAprilTagResult *p)
{
    memset(p, 0, 32);
    if (current != APPLICATION_APRILTAG)
    {
        return false;
    }
    int num = result_summ[1];
    for (int i = 2; i < 2 + num; ++i)
    {
        if (result_summ[i] == id)
        {
            --index;
            if (index == 0)
            {
                readFromAddr(0x1E00 + 0x30 + 0x32 * (i - 2), (uint8_t *)p, 32);
                return true;
            }
        }
    }
    return false;
}

// QRCode functions
bool ExoNaut_AICam::qrCodeDetected(void)
{
    if (current != APPLICATION_QRCODE)
    {
        return false;
    }
    return result_summ[1] > 0 ? true : false;
}

int ExoNaut_AICam::qrCodeDataLength(void)
{
    WonderCamQrCodeResultSumm *p = (WonderCamQrCodeResultSumm *)result_summ;
    if (current != APPLICATION_QRCODE)
    {
        return 0;
    }
    return (int)p->len;
}

int ExoNaut_AICam::qrCodeData(uint8_t *buf)
{
    int len, ret;
    WonderCamQrCodeResultSumm *p = (WonderCamQrCodeResultSumm *)result_summ;
    len = p->len;
    buf[0] = '\0';
    if (current != APPLICATION_QRCODE)
    {
        return 0;
    }
    if (!qrCodeDetected())
    {
        return 0;
    }
    ret = readFromAddr(0x1800 + 48, buf, len);
    return ret;
}

// Barcode functions
bool ExoNaut_AICam::barCodeDetected(void)
{
    if (current != APPLICATION_BARCODE)
    {
        return false;
    }
    return result_summ[1] > 0 ? true : false;
}

int ExoNaut_AICam::barCodeDataLength(void)
{
    WonderCamQrCodeResultSumm *p = (WonderCamQrCodeResultSumm *)result_summ;
    if (current != APPLICATION_BARCODE)
    {
        return 0;
    }
    return (int)p->len;
}

int ExoNaut_AICam::barCodeData(uint8_t *buf)
{
    int len, ret;
    WonderCamQrCodeResultSumm *p = (WonderCamQrCodeResultSumm *)result_summ;
    len = p->len;
    buf[0] = '\0';
    if (current != APPLICATION_BARCODE)
    {
        return 0;
    }
    if (!barCodeDetected())
    {
        return 0;
    }
    ret = readFromAddr(0x1C00 + 48, buf, len);
    return ret;
}

// Is a color recognized?
bool ExoNaut_AICam::anyColorDetected(void)
{
    if (current != APPLICATION_COLORDETECT)
    {
        return false;
    }
    return result_summ[1] > 0 ? true : false;
}

// Number of colors recognized
int ExoNaut_AICam::numOfColorDetected(void)
{
    if (current != APPLICATION_COLORDETECT)
    {
        return 0;
    }
    return result_summ[1];
}

// Whether the specified color is recognized
bool ExoNaut_AICam::colorIdDetected(uint8_t id)
{
    if (current != APPLICATION_COLORDETECT)
    {
        return false;
    }
    int num = result_summ[1];
    for (int i = 2; i < 2 + num; ++i)
    {
        if (result_summ[i] == id)
        {
            return true;
        }
    }
    return false;
}

// Get the position data of the specified recognized color
bool ExoNaut_AICam::colorId(uint8_t id, WonderCamColorDetectResult *p)
{
    memset(p, 0, sizeof(WonderCamColorDetectResult));
    if (current != APPLICATION_COLORDETECT)
    {
        return false;
    }
    if (!anyColorDetected())
    {
        return false;
    }
    int num = result_summ[1];
    for (int i = 2; i < 2 + num; ++i)
    {
        if (result_summ[i] == id)
        {
            int r = readFromAddr(0x1000 + 48 + (16 * (i - 2)), (uint8_t *)p, 16);
            return r == 16 ? true : false;
        }
    }
    return false;
}

// Is the line recognized?
bool ExoNaut_AICam::anyLineDetected(void)
{
    if (current != APPLICATION_LINEFOLLOW)
    {
        return false;
    }
    return result_summ[1] > 0 ? true : false;
}

// Number of lines identified
int ExoNaut_AICam::numOfLineDetected(void)
{
    if (current != APPLICATION_LINEFOLLOW)
    {
        return 0;
    }
    return result_summ[1];
}

// Whether the specified line is recognized
bool ExoNaut_AICam::lineIdDetected(uint8_t id)
{
    if (current != APPLICATION_LINEFOLLOW)
    {
        return false;
    }
    int num = result_summ[1];
    for (int i = 2; i < 2 + num; ++i)
    {
        if (result_summ[i] == id)
        {
            return true;
        }
    }
    return false;
}

// Get the specified recognized line position data
bool ExoNaut_AICam::lineId(uint8_t id, WonderCamLineResult *p)
{
    memset(p, 0, sizeof(WonderCamLineResult));
    if (current != APPLICATION_LINEFOLLOW)
    {
        return false;
    }
    if (!anyLineDetected())
    {
        return false;
    }
    int num = result_summ[1];
    for (int i = 2; i < 2 + num; ++i)
    {
        if (result_summ[i] == id)
        {
            int r = readFromAddr(0x1400 + 48 + (16 * (i - 2)), (uint8_t *)p, 16);
            if (r != 16)
            {
                return false;
            }
            p->angle = p->angle > 90 ? p->angle - 180 : p->angle;
            p->offset = abs(p->offset) - 160;
            return true;
        }
    }
    return false;
}

// Landmark Recognition Functions

bool ExoNaut_AICam::anyLandmarkDetected(void)
{
    if (current != APPLICATION_LANDMARK)
    {
        return false;
    }
    return result_summ[1] > 0 ? true : false;
}

int ExoNaut_AICam::numOfLandmarksDetected(void)
{
    if (current != APPLICATION_LANDMARK)
    {
        return 0;
    }
    return result_summ[1];
}

bool ExoNaut_AICam::landmarkIdDetected(uint8_t id)
{
    if (current != APPLICATION_LANDMARK)
    {
        return false;
    }
    int num = result_summ[1];
    for (int i = 2; i < 2 + num; ++i)
    {
        if (result_summ[i] == id)
        {
            return true;
        }
    }
    return false;
}

int ExoNaut_AICam::numOfLandmarkIdDetected(uint8_t id)
{
    if (current != APPLICATION_LANDMARK)
    {
        return 0;
    }
    int count = 0;
    int num = result_summ[1];
    for (int i = 2; i < 2 + num; ++i)
    {
        if (result_summ[i] == id)
        {
            ++count;
        }
    }
    return count;
}

bool ExoNaut_AICam::getLandmarkById(uint8_t id, WonderCamLandmarkResult *p)
{
    memset(p, 0, sizeof(WonderCamLandmarkResult));
    if (current != APPLICATION_LANDMARK)
    {
        return false;
    }
    if (!anyLandmarkDetected())
    {
        return false;
    }
    int num = result_summ[1];
    for (int i = 2; i < 2 + num; ++i)
    {
        if (result_summ[i] == id)
        {
            int r = readFromAddr(0x0D80 + 48 + (16 * (i - 2)), (uint8_t *)p, 16);
            return r == 16 ? true : false;
        }
    }
    return false;
}

// Updated landmark probability methods to use correct address
int ExoNaut_AICam::landmarkIdWithMaxProb()
{
    if (current != APPLICATION_LANDMARK)
    {
        return 0;
    }

    // Read directly from the correct address
    uint8_t landmarkData[4];
    readFromAddr(0x0D80, landmarkData, 4);

    // Return the ID from the second byte
    return (int8_t)landmarkData[1];
}

float ExoNaut_AICam::landmarkMaxProb()
{
    uint16_t prob_u16;
    if (current != APPLICATION_LANDMARK)
    {
        return 0;
    }

    // Read directly from the correct address
    uint8_t landmarkData[4];
    readFromAddr(0x0D80, landmarkData, 4);

    // Combine bytes for probability (little-endian: bytes 2-3)
    prob_u16 = (landmarkData[3] << 8) | landmarkData[2];

    return ((float)((int)(prob_u16))) / 10000.0;
}

float ExoNaut_AICam::landmarkProbOfId(uint8_t id)
{
    uint16_t prob_u16;
    if (current != APPLICATION_LANDMARK)
    {
        return 0;
    }

    // Read from the correct address for the specific ID
    uint8_t landmarkData[4];
    readFromAddr(0x0D80 + 16 + (id - 1) * 4, landmarkData, 4);

    // First two bytes contain the probability
    prob_u16 = (landmarkData[1] << 8) | landmarkData[0];

    return ((float)((int)(prob_u16))) / 10000.0;
}

// Number recognition methods (added from WonderCam implementation)
int ExoNaut_AICam::numberWithMaxProb()
{
    if (current != APPLICATION_NUMBER_REC)
    {
        return 0;
    }
    return (int8_t)result_summ[1];
}

float ExoNaut_AICam::numberMaxProb()
{
    uint16_t prob_u16;
    if (current != APPLICATION_NUMBER_REC)
    {
        return 0;
    }
    memcpy(&prob_u16, &result_summ[2], 2);
    return ((float)((int)(prob_u16))) / 10000.0;
}

float ExoNaut_AICam::numberProbOfId(uint8_t id)
{
    uint16_t prob_u16;
    if (current != APPLICATION_NUMBER_REC)
    {
        return 0;
    }
    memcpy(&prob_u16, &result_summ[16 + (id - 1) * 4], 2);
    return ((float)((int)(prob_u16))) / 10000.0;
}

// Update results
bool ExoNaut_AICam::updateResult(void)
{
    readFromAddr(0x0035, &current, 1);
    switch (current)
    {
    case APPLICATION_FACEDETECT:
    {
        readFromAddr(0x0400, result_summ, 48);
        break;
    };
    case APPLICATION_OBJDETECT:
    {
        readFromAddr(0x0800, result_summ, 48);
        break;
    }
    case APPLICATION_CLASSIFICATION:
    {
        readFromAddr(0x0C00, result_summ, 128);
        break;
    }
    case APPLICATION_NUMBER_REC:
    {
        readFromAddr(0x0D00, result_summ, 128);
        break;
    }
    case APPLICATION_LANDMARK:
    {
        // Updated to read from the correct address for landmarks
        readFromAddr(0x0D80, result_summ, 48);
        break;
    }
    case APPLICATION_FEATURELEARNING:
    {
        readFromAddr(0x0E00, result_summ, 64);
        break;
    }
    case APPLICATION_COLORDETECT:
    {
        readFromAddr(0x1000, result_summ, 48);
        break;
    }
    case APPLICATION_LINEFOLLOW:
    {
        readFromAddr(0x1400, result_summ, 48);
        break;
    }
    case APPLICATION_APRILTAG:
    {
        readFromAddr(0x1E00, result_summ, 48);
        break;
    }
    case APPLICATION_QRCODE:
    {
        readFromAddr(0x1800, result_summ, 48);
        break;
    }
    case APPLICATION_BARCODE:
    {
        readFromAddr(0x1C00, result_summ, 48);
        break;
    }
    default:
    {
        break;
    }
    }
    return true;
}

/* -------------------------------
   New AprilTag helper functions
   ------------------------------- */

// Print details of all detected tags to the Serial monitor
bool ExoNaut_AICam::printAllTagDetails(void)
{
    if (current != APPLICATION_APRILTAG)
    {
        Serial.println("Error: Camera not in AprilTag mode");
        return false;
    }
    uint8_t tagCount = result_summ[1];
    Serial.print("Detected ");
    Serial.print(tagCount);
    Serial.println(" AprilTags:");
    for (int i = 0; i < tagCount; i++)
    {
        WonderCamAprilTagResult tag;
        // Each tag's detail is located at address: base + 0x30 + (0x32 * tag_index)
        int addrOffset = 0x30 + 0x32 * i;
        if (readFromAddr(0x1E00 + addrOffset, (uint8_t *)&tag, sizeof(tag)) != sizeof(tag))
        {
            Serial.println("Error reading tag data");
            continue;
        }
        Serial.print("Tag ");
        Serial.print(i);
        Serial.print(": ID=");
        Serial.print(result_summ[2 + i]);
        Serial.print(", x=");
        Serial.print(tag.x);
        Serial.print(", y=");
        Serial.print(tag.y);
        Serial.print(", w=");
        Serial.print(tag.w);
        Serial.print(", h=");
        Serial.print(tag.h);
        Serial.print(", x_t=");
        Serial.print(tag.x_t);
        Serial.print(", x_r=");
        Serial.print(tag.x_r);
        Serial.print(", y_t=");
        Serial.print(tag.y_t);
        Serial.print(", y_r=");
        Serial.print(tag.y_r);
        Serial.print(", z_t=");
        Serial.print(tag.z_t);
        Serial.print(", z_r=");
        Serial.println(tag.z_r);
    }
    return true;
}

// Get the details for a specific tag id and store them in the provided structure
bool ExoNaut_AICam::getTagInfo(uint16_t tagId, WonderCamAprilTagResult *tag)
{
    if (current != APPLICATION_APRILTAG)
    {
        return false;
    }
    uint8_t tagCount = result_summ[1];
    for (int i = 0; i < tagCount; i++)
    {
        // Tag IDs are stored starting at result_summ[2]
        if (result_summ[2 + i] == tagId)
        {
            int addrOffset = 0x30 + 0x32 * i;
            if (readFromAddr(0x1E00 + addrOffset, (uint8_t *)tag, sizeof(WonderCamAprilTagResult)) != sizeof(WonderCamAprilTagResult))
            {
                return false;
            }
            return true;
        }
    }
    return false;
}

// Estimate the distance to a tag using its width; a simple pinhole camera model is assumed.
// The focal length is an assumed constant (example value: 500.0) and realTagSize is in the same units as desired output.
float ExoNaut_AICam::estimateTagDistance(WonderCamAprilTagResult *tag, float realTagSize)
{
    if (tag->w == 0)
        return -1;
    const float focalLength = 500.0; // Example focal length value; adjust as needed
    return (realTagSize * focalLength) / tag->w;
}

// Retrieve the orientation of a tag. Here, we simply return the z-axis rotation value.
float ExoNaut_AICam::getTagOrientation(WonderCamAprilTagResult *tag)
{
    return tag->z_r;
}

// List all detected tag IDs by printing them to the Serial monitor.
void ExoNaut_AICam::listDetectedTagIds(void)
{
    if (current != APPLICATION_APRILTAG)
    {
        Serial.println("Error: Camera not in AprilTag mode");
        return;
    }
    uint8_t tagCount = result_summ[1];
    Serial.print("Detected Tag IDs: ");
    for (int i = 0; i < tagCount; i++)
    {
        Serial.print(result_summ[2 + i]);
        if (i < tagCount - 1)
            Serial.print(", ");
    }
    Serial.println();
}

void ExoNaut_AICam::printFaceTable()
{
    if (current != APPLICATION_FACEDETECT)
    {
        Serial.println("Error: Camera is not in Face Detection mode.");
        return;
    }

    updateResult(); // Always refresh data first

    int totalFaces = numOfTotalFaceDetected();
    if (totalFaces == 0)
    {
        Serial.println("No faces detected.");
        return;
    }

    Serial.println("----------------------------------------------");
    Serial.printf("| %-10s | %5s | %5s | %5s | %5s |\n", "Face ID", "X", "Y", "W", "H");
    Serial.println("----------------------------------------------");

    // Print learned faces
    for (uint8_t id = 1; id <= 29; id++)
    {
        if (faceOfIdDetected(id))
        {
            WonderCamFaceDetectResult face;
            if (getFaceOfId(id, &face))
            {
                Serial.printf("| %-10d | %5d | %5d | %5d | %5d |\n",
                              id, face.x, face.y, face.w, face.h);
            }
        }
    }

    // Print unlearned faces
    uint8_t unlearnedIndex = 1;
    int unlearnedFaces = numOfTotalUnlearnedFaceDetected();
    for (int i = 0; i < unlearnedFaces; i++)
    {
        WonderCamFaceDetectResult face;
        if (getFaceOfIndex(unlearnedIndex++, &face))
        {
            Serial.printf("| %-10s | %5d | %5d | %5d | %5d |\n",
                          "Unknown", face.x, face.y, face.w, face.h);
        }
    }

    Serial.println("----------------------------------------------");
}
