#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>

const float PI = 3.1416;
float wheelAngle = 0.0f;
float flutterTime = 0.0f;
bool isNight = false;

float personX = 400;
float personY = 100;


// Crowd data
const int CROWD_SIZE = 20;
float crowdX[CROWD_SIZE];
float crowdY[CROWD_SIZE];
bool crowdInitialized = false;

const int LEAF_COUNT = 15;
float leafX[LEAF_COUNT];
float leafY[LEAF_COUNT];
float leafSpeed[LEAF_COUNT];
bool leafInitialized = false;

float userKiteX = 200;
float userKiteY = 400;



// Bird positions and animation variables
const int NUM_BIRDS = 10;
float birdX[NUM_BIRDS];
float birdY[NUM_BIRDS];

void initBirds() {
    for (int i = 0; i < NUM_BIRDS; ++i) {
        birdX[i] = rand() % 800;                  // random start x
        birdY[i] = 450 + rand() % 120;            // random y height in sky
    }
}

void drawBird(float x, float y) {
    glColor3f(0.0f, 0.0f, 0.0f);  // black birds
    glBegin(GL_LINES);
    glVertex2f(x, y);
    glVertex2f(x + 5, y + 5);
    glVertex2f(x + 5, y + 5);
    glVertex2f(x + 10, y);
    glEnd();
}

void drawAllBirds() {
    for (int i = 0; i < NUM_BIRDS; ++i) {
        drawBird(birdX[i], birdY[i]);
        birdX[i] += 1.0f;  // move right

        if (birdX[i] > 800) {
            birdX[i] = -20;                    // reset to left
            birdY[i] = 450 + rand() % 120;     // new height
        }
    }
}

//  Draw filled circle
void drawCircle(float cx, float cy, float r, int segments = 100) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < segments; ++i) {
        float theta = 2.0f * PI * float(i) / float(segments);
        float x = r * cos(theta);
        float y = r * sin(theta);
        glVertex2f(cx + x, cy + y);
    }
    glEnd();
}


// Sky with sun or moon
void drawSky() {
    if (isNight)
        glColor3f(0.05f, 0.0f, 0.2f); // night
    else
        glColor3f(0.6f, 0.9f, 1.0f); // day

    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(800, 0);
    glVertex2f(800, 600);
    glVertex2f(0, 600);
    glEnd();

    if (isNight)
        glColor3f(1.0f, 1.0f, 0.8f); // moon
    else
        glColor3f(1.0f, 1.0f, 0.0f); // sun

    drawCircle(700, 520, 30);
}

// Ground
void drawGround() {
    if (isNight)
        glColor3f(0.0f, 0.3f, 0.1f);
    else
        glColor3f(0.0f, 0.6f, 0.2f);

    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(800, 0);
    glVertex2f(800, 200);
    glVertex2f(0, 200);
    glEnd();
}

// Enhanced Gallery with tiered seats
void drawGallery() {
    // Base structure
    glColor3f(0.4f, 0.4f, 0.6f);
    glBegin(GL_POLYGON); // Curved back wall
    glVertex2f(250, 200);
    glVertex2f(550, 200);
    glVertex2f(530, 280);
    glVertex2f(270, 280);
    glEnd();

    // Tiered seating (curved)
    for (int i = 0; i < 4; i++) {
        float y1 = 210 + i * 15;
        float y2 = y1 + 10;
        glColor3f(0.2f + 0.1f * i, 0.3f + 0.1f * i, 0.8f); // blue shades
        glBegin(GL_QUADS);
        glVertex2f(265 + i * 5, y1);
        glVertex2f(535 - i * 5, y1);
        glVertex2f(530 - i * 5, y2);
        glVertex2f(270 + i * 5, y2);
        glEnd();
    }

    // Vertical pillars
    glColor3f(0.5f, 0.5f, 0.5f);
    for (int i = 0; i <= 4; i++) {
        float x = 270 + i * 50;
        glBegin(GL_QUADS);
        glVertex2f(x - 2, 200);
        glVertex2f(x + 2, 200);
        glVertex2f(x + 2, 280);
        glVertex2f(x - 2, 280);
        glEnd();
    }

    // Canopy / roof
    glColor3f(0.9f, 0.9f, 0.9f); // white
    glBegin(GL_POLYGON);
    glVertex2f(260, 280);
    glVertex2f(540, 280);
    glVertex2f(500, 300);
    glVertex2f(300, 300);
    glEnd();

    // Scoreboard/Banner
    glColor3f(0.8f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(290, 305);
    glVertex2f(500, 305);
    glVertex2f(500, 320);
    glVertex2f(290, 320);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(340, 310);
    const char* msg = "Welcome to Boishakhi Mela in AIUB!";
    for (const char* c = msg; *c; ++c)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
}

// Ferris wheel (right side)
void drawFerrisWheel() {
    float cx = 650, cy = 280, r = 70;

    glColor3f(1.0f, 0.0f, 0.0f);
    drawCircle(cx, cy, r);

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    for (int i = 0; i < 8; ++i) {
        float angle = i * 2 * PI / 8 + wheelAngle;
        float x = cx + r * cos(angle);
        float y = cy + r * sin(angle);
        glVertex2f(cx, cy);
        glVertex2f(x, y);
    }
    glEnd();

    for (int i = 0; i < 8; ++i) {
        float angle = i * 2 * PI / 8 + wheelAngle;
        float x = cx + r * cos(angle);
        float y = cy + r * sin(angle);
        glColor3f(1.0f, 1.0f, 0.0f);
        drawCircle(x, y, 5);
    }

    glColor3f(0.4f, 0.2f, 0.1f);
    glBegin(GL_QUADS);
    glVertex2f(cx - 20, 200);
    glVertex2f(cx + 20, 200);
    glVertex2f(cx + 20, 240);
    glVertex2f(cx - 10, 240);
    glEnd();
}

void drawFerrisWheelPoster() {
    // Poster background rectangle
    glColor3f(0.9f, 0.6f, 0.1f);
    glBegin(GL_QUADS);
    glVertex2f(650, 350);  // adjust based on wheel position
    glVertex2f(650, 350);
    glVertex2f(650, 370);
    glVertex2f(650, 370);
    glEnd();

    // Text "Wheel"
    glColor3f(0.0f, 0.0f, 0.0f); // black text
    glRasterPos2f(640, 357);
    const char* label = "Wheel";
    for (const char* c = label; *c; ++c)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
}

// Crowd positions generated once
void drawCrowd() {
    if (!crowdInitialized) {
        srand(1);
        for (int i = 0; i < CROWD_SIZE; i++) {
            crowdX[i] = 160 + rand() % 500;
            crowdY[i] = 20 + rand() % 160;
        }
        crowdInitialized = true;
    }

    for (int i = 0; i < CROWD_SIZE; i++) {
        float x = crowdX[i];
        float y = crowdY[i];
        glColor3f(0.0f, 0.0f, 0.0f);
        drawCircle(x, y, 2);
        glBegin(GL_LINES);
        glVertex2f(x, y); glVertex2f(x, y - 6);
        glVertex2f(x, y - 6); glVertex2f(x - 2, y - 10);
        glVertex2f(x, y - 6); glVertex2f(x + 2, y - 10);
        glEnd();
    }
}

// Draw a single stall with roof, body, Alpona, and label
void drawBoishakhStall(float x, float y, float width = 100, float height = 50, const char* stallName = "Stall") {
    // Body
    glColor3f(1.0f, 0.8f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();

    // Roof
    glColor3f(0.8f, 0.1f, 0.1f);
    glBegin(GL_TRIANGLES);
    glVertex2f(x - 10, y + height);
    glVertex2f(x + width + 10, y + height);
    glVertex2f(x + width / 2, y + height + 15);
    glEnd();

    // Alpona
    glColor3f(1.0f, 1.0f, 1.0f);
    drawCircle(x + width / 2, y + height + 5, 6, 50);

    // Name
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(x + 10, y + height / 2 - 4);
    for (const char* c = stallName; *c != '\0'; ++c)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
}

// Draw 5 vertically aligned stalls on left side
void drawBoishakhStalls() {
    float baseX = 30;
    float baseY = 180;
    float spacing = -55;

    const char* stallNames[5] = {
        "Panjabi & Sharee",
        "Mishti Stall",
        "Naksha",
        "Pitha",
        "Folk Art"
    };

    for (int i = 0; i < 5; ++i) {
        float y = baseY + i * spacing;
        drawBoishakhStall(baseX, y, 100, 50, stallNames[i]);
    }
}

// Animated festoon flutter
void drawAnimatedFestoon(float x1, float y1, float x2, float y2, int flags = 10, float time = 0.0f) {
    float dx = (x2 - x1) / flags;
    float dy = (y2 - y1) / flags;

    for (int i = 0; i < flags; ++i) {
        float fx = x1 + i * dx;
        float fy = y1 + i * dy;

        float wave = sinf(time + i * 0.5f) * 5.0f;

        float r = 0.8f;
        float g = 0.2f + 0.3f * sinf(i);
        float b = 0.3f + 0.5f * cosf(i);
        glColor3f(r, g, b);

        glBegin(GL_TRIANGLES);
        glVertex2f(fx, fy + wave);
        glVertex2f(fx + dx / 2, fy - 12 + wave);
        glVertex2f(fx + dx, fy + wave);
        glEnd();
    }
}

// Draw visitor near stall
void drawVisitorAtStall(float x, float y, bool hasBalloon = false) {
    glColor3f(0.8f, 0.6f, 0.4f);
    drawCircle(x, y, 3);

    glColor3f(0.1f, 0.5f, 0.2f);
    glBegin(GL_LINES);
    glVertex2f(x, y); glVertex2f(x, y - 10);
    glVertex2f(x, y - 10); glVertex2f(x - 3, y - 15);
    glVertex2f(x, y - 10); glVertex2f(x + 3, y - 15);
    glVertex2f(x, y - 6); glVertex2f(x - 3, y - 8);
    glVertex2f(x, y - 6); glVertex2f(x + 3, y - 8);
    glEnd();

    if (hasBalloon) {
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_LINES);
        glVertex2f(x + 3, y);
        glVertex2f(x + 3, y + 20);
        glEnd();
        drawCircle(x + 3, y + 25, 5);
    }
}

// Draw 3-4 buyers near stalls (static)
void drawBuyersNearStalls() {
    drawVisitorAtStall(45, 170, false);
    drawVisitorAtStall(45, 130, true);
    drawVisitorAtStall(45, 90, false);
    drawVisitorAtStall(45, 50, true);
}

// Draw a kite at (cx, cy) with given size
void drawKite(float cx, float cy, float size = 30.0f) {
    // Kite body (diamond shape)
    glColor3f(1.0f, 0.2f, 0.2f);  // bright red
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy + size / 2);          // top
    glVertex2f(cx + size / 2, cy);          // right
    glVertex2f(cx, cy - size / 2);          // bottom
    glVertex2f(cx - size / 2, cy);          // left
    glEnd();

    // String from kite down to ground (fixed point)
    glColor3f(0.5f, 0.35f, 0.05f); // brownish string
    glBegin(GL_LINES);
    glVertex2f(cx, cy - size / 2);
    glVertex2f(cx, 100);  // fixed y on ground (around 100)
    glEnd();
}

// Variables for kite animation
float kite1X = 400, kite1Y = 500;
float kite2X = 600, kite2Y = 480;
float kiteTime = 0.0f;

// Call this inside your display()
void drawKites() {
    // Animate kite positions using sine wave for smooth movement
    float kite1NewX = kite1X + 30 * sin(kiteTime * 0.8f);
    float kite1NewY = kite1Y + 10 * cos(kiteTime * 1.1f);

    float kite2NewX = kite2X + 25 * sin(kiteTime * 1.2f + 1.5f);
    float kite2NewY = kite2Y + 15 * cos(kiteTime * 0.9f + 1.5f);

    drawKite(kite1NewX, kite1NewY);
    drawKite(kite2NewX, kite2NewY);

    kiteTime += 0.02f;
}

float singerHandAngle = 0.0f;

void drawFolkMusicStage() {
    // Stage position (right side, east-west facing)
    float stageX1 = 580, stageY1 = 60;
    float stageX2 = 740, stageY2 = 100;

    // Stage Platform
    glColor3f(0.6f, 0.3f, 0.1f);
    glBegin(GL_QUADS);
    glVertex2f(stageX1, stageY1);
    glVertex2f(stageX2, stageY1);
    glVertex2f(stageX2, stageY2);
    glVertex2f(stageX1, stageY2);
    glEnd();

    // Stage Banner
    glColor3f(0.8f, 0.1f, 0.1f);
    glBegin(GL_QUADS);
    glVertex2f(stageX1 + 20, stageY2 + 5);
    glVertex2f(stageX2 - 20, stageY2 + 5);
    glVertex2f(stageX2 - 20, stageY2 + 20);
    glVertex2f(stageX1 + 20, stageY2 + 20);
    glEnd();

    // Banner text
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(stageX1 + 40, stageY2 + 10);
    const char* label = "Folk Music Stage";
    for (const char* c = label; *c; ++c)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);

    // Singer (centered)
    float singerX = (stageX1 + stageX2) / 2;
    float singerY = stageY2 + 15;

    glColor3f(0.9f, 0.6f, 0.5f); // head
    drawCircle(singerX, singerY, 7);

    glBegin(GL_LINES); // body
    glVertex2f(singerX, singerY - 7); glVertex2f(singerX, singerY - 25);

    // Arms
    glVertex2f(singerX, singerY - 10); glVertex2f(singerX - 10, singerY - 15); // left
    float waveY = singerY - 10 + 4 * sin(singerHandAngle);
    glVertex2f(singerX, singerY - 10); glVertex2f(singerX + 10, waveY); // right
    glEnd();

    // Tabla (left)
    glColor3f(0.8f, 0.5f, 0.1f);
    drawCircle(singerX - 25, stageY1 + 12, 6);
    drawCircle(singerX - 10, stageY1 + 12, 6);

    // Flute (right)
    glColor3f(0.9f, 0.7f, 0.1f);
    glBegin(GL_QUADS);
    glVertex2f(singerX + 15, stageY1 + 10);
    glVertex2f(singerX + 55, stageY1 + 10);
    glVertex2f(singerX + 55, stageY1 + 13);
    glVertex2f(singerX + 15, stageY1 + 13);
    glEnd();

    // Festoon
    drawAnimatedFestoon(stageX1, stageY2 + 25, stageX2, stageY2 + 25, 10, flutterTime);
}

// Draw a single cloud at (x, y)
void drawCloud(float x, float y) {
    glColor3f(1.0f, 1.0f, 1.0f);  // white color
    drawCircle(x, y, 15);
    drawCircle(x + 15, y + 5, 12);
    drawCircle(x - 15, y + 5, 12);
    drawCircle(x + 7, y + 10, 10);
}

void drawClouds() {
    if (!isNight) {  // only draw clouds in daytime
        drawCloud(120, 500);
        drawCloud(300, 540);
        drawCloud(500, 520);
        drawCloud(650, 560);
    }
}

void drawTree(float x, float y, float trunkHeight = 30, float crownRadius = 15) {
    // Trunk
    glColor3f(0.5f, 0.35f, 0.05f);
    glBegin(GL_QUADS);
    glVertex2f(x - 2, y);
    glVertex2f(x + 2, y);
    glVertex2f(x + 2, y + trunkHeight);
    glVertex2f(x - 2, y + trunkHeight);
    glEnd();

    // Crown (leaves)
    glColor3f(0.0f, 0.6f, 0.2f);
    drawCircle(x, y + trunkHeight + 5, crownRadius);
    drawCircle(x - 5, y + trunkHeight + 10, crownRadius - 3);
    drawCircle(x + 5, y + trunkHeight + 10, crownRadius - 3);
}

void drawGallerySideTrees() {
    drawTree(180, 200, 35, 13);
    drawTree(200, 200, 40, 15);
    drawTree(220, 200, 32, 12);
    drawTree(240, 200, 38, 14);
}

void initFallingLeaves() {
    for (int i = 0; i < LEAF_COUNT; ++i) {
        leafX[i] = 180 + rand() % 60;  // near gallery trees
        leafY[i] = 250 + rand() % 100;
        leafSpeed[i] = 0.5f + static_cast<float>(rand() % 100) / 200.0f;  // 0.5 to 1.0
    }
    leafInitialized = true;
}

void drawLeaf(float x, float y) {
    glColor3f(0.3f, 0.7f, 0.1f);
    glBegin(GL_TRIANGLES);
    glVertex2f(x, y);
    glVertex2f(x - 2, y - 4);
    glVertex2f(x + 2, y - 4);
    glEnd();
}


void drawLantern(float x, float y, float flicker) {
    // Lantern body (circle)
    glColor3f(1.0f, 0.5f + 0.3f * flicker, 0.2f); // orange flickering color
    drawCircle(x, y, 7);

    // Lantern top hook
    glColor3f(0.3f, 0.1f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(x, y + 7);
    glVertex2f(x, y + 15);
    glEnd();
}

void drawLanternsAboveStage() {
    float startX = 580;
    float endX = 740;
    float yLine = 110; // just above the stage banner

    // Draw the string
    glColor3f(0.5f, 0.3f, 0.1f);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2f(startX, yLine);
    glVertex2f(endX, yLine);
    glEnd();

    // Draw lanterns spaced along the string
    int lanternCount = 6;
    for (int i = 0; i < lanternCount; ++i) {
        float x = startX + i * (endX - startX) / (lanternCount - 1);
        float flicker = sin(flutterTime * 5 + i);
        drawLantern(x, yLine - 10, flicker);
    }
}

// Draw a person wearing Panjabi
void drawPanjabiPerson(float x, float y) {
    // Head
    glColor3f(1.0f, 0.8f, 0.6f);
    drawCircle(x, y, 4);

    // Body (rectangle, Panjabi color)
    glColor3f(0.2f, 0.4f, 0.9f);  // Blue Panjabi
    glBegin(GL_QUADS);
    glVertex2f(x - 6, y - 12);
    glVertex2f(x + 6, y - 12);
    glVertex2f(x + 6, y - 25);
    glVertex2f(x - 6, y - 25);
    glEnd();

    // Legs (lines)
    glColor3f(0.1f, 0.1f, 0.1f);
    glBegin(GL_LINES);
    glVertex2f(x - 3, y - 25); glVertex2f(x - 3, y - 35);
    glVertex2f(x + 3, y - 25); glVertex2f(x + 3, y - 35);
    glEnd();
}

// Draw a person wearing Saree (female figure)
void drawSareePerson(float x, float y) {
    // Head
    glColor3f(1.0f, 0.8f, 0.6f);
    drawCircle(x, y, 4);

    // Body (triangle to represent saree drape)
    glColor3f(0.9f, 0.1f, 0.1f); // Red Saree
    glBegin(GL_TRIANGLES);
    glVertex2f(x, y - 12);
    glVertex2f(x - 8, y - 35);
    glVertex2f(x + 8, y - 35);
    glEnd();

    // Arms (lines)
    glColor3f(1.0f, 0.8f, 0.6f);
    glBegin(GL_LINES);
    glVertex2f(x - 6, y - 15); glVertex2f(x - 12, y - 22);
    glVertex2f(x + 6, y - 15); glVertex2f(x + 12, y - 22);
    glEnd();
}

// Draw a person wearing Lungis (male lower garment)
void drawLungiPerson(float x, float y) {
    // Head
    glColor3f(1.0f, 0.8f, 0.6f);
    drawCircle(x, y, 4);

    // Body (rectangle upper, white panjabi)
    glColor3f(0.95f, 0.95f, 0.95f);
    glBegin(GL_QUADS);
    glVertex2f(x - 6, y - 12);
    glVertex2f(x + 6, y - 12);
    glVertex2f(x + 6, y - 22);
    glVertex2f(x - 6, y - 22);
    glEnd();

    // Lungis (colorful lower part)
    glColor3f(0.9f, 0.7f, 0.1f);  // Yellow Lungis
    glBegin(GL_QUADS);
    glVertex2f(x - 6, y - 22);
    glVertex2f(x + 6, y - 22);
    glVertex2f(x + 6, y - 35);
    glVertex2f(x - 6, y - 35);
    glEnd();

    // Legs (lines)
    glColor3f(0.1f, 0.1f, 0.1f);
    glBegin(GL_LINES);
    glVertex2f(x - 3, y - 35); glVertex2f(x - 3, y - 45);
    glVertex2f(x + 3, y - 35); glVertex2f(x + 3, y - 45);
    glEnd();
}

// Draw a child holding a balloon
void drawChildWithBalloon(float x, float y) {
    // Smaller head
    glColor3f(1.0f, 0.8f, 0.6f);
    drawCircle(x, y, 3);

    // Body
    glColor3f(0.8f, 0.2f, 0.2f);  // Red T-shirt
    glBegin(GL_QUADS);
    glVertex2f(x - 4, y - 8);
    glVertex2f(x + 4, y - 8);
    glVertex2f(x + 4, y - 18);
    glVertex2f(x - 4, y - 18);
    glEnd();

    // Legs
    glColor3f(0.1f, 0.1f, 0.1f);
    glBegin(GL_LINES);
    glVertex2f(x - 2, y - 18); glVertex2f(x - 2, y - 25);
    glVertex2f(x + 2, y - 18); glVertex2f(x + 2, y - 25);
    glEnd();

    // Balloon string
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(x + 5, y - 6);
    glVertex2f(x + 5, y + 10);
    glEnd();

    // Balloon (circle)
    glColor3f(1.0f, 0.0f, 0.0f);
    drawCircle(x + 5, y + 15, 5);
}
void drawStaticPerson(float x, float y, bool isFemale = false, bool hasBalloon = false) {
    // Head
    glColor3f(0.9f, 0.7f, 0.5f);
    drawCircle(x, y + 10, 3);

    // Body color based on gender
    if (isFemale) glColor3f(1.0f, 0.3f, 0.6f);  // Saree - pink
    else glColor3f(0.2f, 0.6f, 1.0f);           // Panjabi - blue

    // Body
    glBegin(GL_LINES);
    glVertex2f(x, y + 7); glVertex2f(x, y - 5);           // torso
    glVertex2f(x, y + 2); glVertex2f(x - 3, y - 3);       // left leg
    glVertex2f(x, y + 2); glVertex2f(x + 3, y - 3);       // right leg
    glVertex2f(x, y + 5); glVertex2f(x - 3, y + 1);       // left arm
    glVertex2f(x, y + 5); glVertex2f(x + 3, y + 1);       // right arm
    glEnd();

    // Balloon or toy
    if (hasBalloon) {
        glColor3f(1.0f, 0.0f, 0.0f);  // red balloon
        glBegin(GL_LINES);
        glVertex2f(x + 2, y + 5);
        glVertex2f(x + 2, y + 20);
        glEnd();
        drawCircle(x + 2, y + 25, 3);
    }
}
void drawFestivalVisitors() {
    float startX = 160;
    float startY = 60;
    for (int i = 0; i < 20; ++i) {
        float x = startX + (i % 10) * 18;
        float y = startY + (i / 10) * 25;

        bool isFemale = (i % 3 == 0);
        bool hasBalloon = (i % 4 == 0);

        drawStaticPerson(x, y, isFemale, hasBalloon);
    }
}

void drawBoundaryWall() {
    glColor3f(0.4f, 0.2f, 0.0f); // brown wall
    glBegin(GL_QUADS);
    glVertex2f(0, 200);
    glVertex2f(800, 200);
    glVertex2f(800, 220);
    glVertex2f(0, 220);
    glEnd();
}

void drawFlowerTree(float x, float y, float trunkHeight = 30, float crownRadius = 15) {
    // Trunk
    glColor3f(0.5f, 0.35f, 0.05f); // Brown
    glBegin(GL_QUADS);
    glVertex2f(x - 2, y);
    glVertex2f(x + 2, y);
    glVertex2f(x + 2, y + trunkHeight);
    glVertex2f(x - 2, y + trunkHeight);
    glEnd();

    // Leafy Crown
    glColor3f(0.0f, 0.6f, 0.2f); // Green
    drawCircle(x, y + trunkHeight + 5, crownRadius);
    drawCircle(x - 6, y + trunkHeight + 10, crownRadius - 3);
    drawCircle(x + 6, y + trunkHeight + 10, crownRadius - 3);

    // Flowers (Pink)
    glColor3f(1.0f, 0.4f, 0.6f);
    drawCircle(x - 3, y + trunkHeight + 15, 2);
    drawCircle(x + 4, y + trunkHeight + 13, 2);
    drawCircle(x, y + trunkHeight + 18, 2);
}

void drawBengaliPot(float x, float y) {
    // Pot body
    glColor3f(0.7f, 0.2f, 0.1f); // Terracotta red
    drawCircle(x, y, 6, 30);

    // Neck of pot
    glBegin(GL_QUADS);
    glVertex2f(x - 3, y + 6);
    glVertex2f(x + 3, y + 6);
    glVertex2f(x + 2, y + 10);
    glVertex2f(x - 2, y + 10);
    glEnd();

    // Decorative band
    glColor3f(1.0f, 1.0f, 1.0f);
    drawCircle(x, y, 3, 12);
}

void drawFence(float x1, float y1, int posts = 4, float spacing = 10) {
    for (int i = 0; i < posts; ++i) {
        float x = x1 + i * spacing;

        // Post
        glColor3f(0.5f, 0.3f, 0.1f);
        glBegin(GL_QUADS);
        glVertex2f(x - 1, y1);
        glVertex2f(x + 1, y1);
        glVertex2f(x + 1, y1 + 15);
        glVertex2f(x - 1, y1 + 15);
        glEnd();

        // Top line
        glBegin(GL_LINES);
        glVertex2f(x1, y1 + 15);
        glVertex2f(x1 + posts * spacing, y1 + 15);
        glEnd();
    }
}

void drawFestoonSymbol(float x, float y, const char* type) {
    if (strcmp(type, "kite") == 0) {
        glColor3f(1.0f, 0.0f, 0.0f);  // red kite
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y + 5);
        glVertex2f(x + 5, y);
        glVertex2f(x, y - 5);
        glVertex2f(x - 5, y);
        glEnd();
    } else if (strcmp(type, "fish") == 0) {
        glColor3f(0.1f, 0.5f, 1.0f);  // blue fish
        glBegin(GL_TRIANGLES);
        glVertex2f(x - 4, y);     // left
        glVertex2f(x + 4, y + 3); // top right
        glVertex2f(x + 4, y - 3); // bottom right
        glEnd();
    } else if (strcmp(type, "flower") == 0) {
        glColor3f(1.0f, 0.0f, 1.0f);  // pink flower
        for (int i = 0; i < 6; ++i) {
            float angle = i * 60 * PI / 180;
            drawCircle(x + cos(angle) * 5, y + sin(angle) * 5, 2);
        }
        glColor3f(1.0f, 1.0f, 0.0f);  // center
        drawCircle(x, y, 2);
    }
}

void draw3DFenceSection(float startX, float startY, int slats = 10) {
    glColor3f(0.6f, 0.4f, 0.2f); // wood brown

    for (int i = 0; i < slats; i++) {
        float x = startX + i * 12;
        glBegin(GL_QUADS);
        glVertex2f(x, startY);
        glVertex2f(x + 4, startY);
        glVertex2f(x + 4, startY + 25);
        glVertex2f(x, startY + 25);
        glEnd();
    }

    // Top bar
    glBegin(GL_QUADS);
    glVertex2f(startX, startY + 25);
    glVertex2f(startX + slats * 12, startY + 25);
    glVertex2f(startX + slats * 12, startY + 30);
    glVertex2f(startX, startY + 30);
    glEnd();
}

void drawGalleryBlock(float x, float y, float w = 300, float h = 70) {
    // Front face
    glColor3f(0.4f, 0.4f, 0.6f);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();

    // Top face
    glColor3f(0.3f, 0.3f, 0.5f);
    glBegin(GL_QUADS);
    glVertex2f(x, y + h);
    glVertex2f(x + w, y + h);
    glVertex2f(x + w - 15, y + h + 15);
    glVertex2f(x + 15, y + h + 15);
    glEnd();
}

void drawMovablePerson(float x, float y) {
    // Head
    glColor3f(1.0f, 0.8f, 0.6f);
    drawCircle(x, y, 5);

    // Body
    glColor3f(0.2f, 0.4f, 1.0f); // blue shirt
    glBegin(GL_LINES);
    glVertex2f(x, y - 5); glVertex2f(x, y - 20); // body

    // Arms
    glVertex2f(x, y - 8); glVertex2f(x - 5, y - 15);
    glVertex2f(x, y - 8); glVertex2f(x + 5, y - 15);

    // Legs
    glVertex2f(x, y - 20); glVertex2f(x - 5, y - 30);
    glVertex2f(x, y - 20); glVertex2f(x + 5, y - 30);
    glEnd();
}



// Display callback
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawSky();
    drawClouds();
    drawAllBirds();
    drawKites();
    drawBoundaryWall();
    drawGround();
    draw3DFenceSection(100, 190);
    drawGalleryBlock(250, 200);
    drawGround();
    drawGallery();
    drawFestoonSymbol(270, 290, "kite");
    drawFestoonSymbol(300, 290, "fish");
    drawFestoonSymbol(330, 290, "flower");
    drawFestoonSymbol(360, 290, "kite");
    drawFestoonSymbol(390, 290, "fish");
    drawFestoonSymbol(420, 290, "flower");
    drawFestoonSymbol(450, 290, "kite");
    drawFestoonSymbol(480, 290, "fish");
    drawFestoonSymbol(510, 290, "flower");
    drawMovablePerson(personX, personY);
    drawGallerySideTrees();
    drawFlowerTree(140, 200);
    drawFlowerTree(160, 200);
    drawBengaliPot(135, 200);
    drawBengaliPot(165, 200);
    drawFence(130, 200, 5, 8);
    drawBoishakhStalls();
    drawFerrisWheel();
    drawFerrisWheelPoster();
    drawCrowd();
    drawFolkMusicStage();
    drawLanternsAboveStage();
    drawPanjabiPerson(100, 180);
    drawLungiPerson(180, 185);
    drawSareePerson(260, 180);
    drawChildWithBalloon(300, 160);
    drawChildWithBalloon(340, 150);
    drawBuyersNearStalls();
    drawFestivalVisitors();
    drawAnimatedFestoon(20, 250, 160, 250, 10, flutterTime);

    glFlush();
}

// Timer update callback
void update(int value) {
    wheelAngle += 0.05f;
    flutterTime += 0.1f;

    if (wheelAngle > 2 * PI) wheelAngle -= 2 * PI;


    glutPostRedisplay();
    glutTimerFunc(33, update, 0);
}

// Keyboard input callback
void keyboard(unsigned char key, int x, int y) {
    if (key == 27) {
        exit(0);     // Exit the program
    }

    if (key == 'd' || key == 'D') {
        isNight = !isNight;

        glutPostRedisplay();
    }
}

void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
            personX -= 5;
            break;
        case GLUT_KEY_RIGHT:
            personX += 5;
            break;
        case GLUT_KEY_UP:
            personY += 5;
            break;
        case GLUT_KEY_DOWN:
            personY -= 5;
            break;
    }

    if (personX < 0) personX = 0;
    if (personX > 800) personX = 800;
    if (personY < 0) personY = 0;
    if (personY > 600) personY = 600;

    glutPostRedisplay();
}




// Initialization
void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    gluOrtho2D(0, 800, 0, 600);
     srand(time(0));
    initBirds();
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1080, 630);
    glutCreateWindow("Pohela Boishakh");
    init();
    glutDisplayFunc(display);

    glutKeyboardFunc(keyboard);

    glutSpecialFunc(specialKeys);

    glutTimerFunc(0, update, 0);
    glutMainLoop();
    return 0;
}
