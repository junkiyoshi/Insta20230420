#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);

	this->font.loadFont("fonts/Kazesawa-Bold.ttf", 580, true, true, true);

}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	if (ofGetFrameNum() % 30 == 0) {

		this->location_list.clear();
		
		ofFbo fbo;
		fbo.allocate(ofGetWidth(), ofGetHeight());
		fbo.begin();
		ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);
		ofClear(0);
		ofSetColor(0);

		string word = std::to_string((ofGetFrameNum() / 30) % 10);
		font.drawString(word, font.stringWidth(word) * -0.5, font.stringHeight(word) - 320);

		fbo.end();

		auto span = 20;
		ofPixels pixels;
		fbo.readToPixels(pixels);
		for (int x = 60; x < 700; x += span) {

			for (int y = 0; y < 720; y += span) {

				ofColor color = pixels.getColor(x, y);
				if (color != ofColor(0, 0)) {

					this->location_list.push_back(glm::vec3(x - ofGetWidth() * 0.5, y - ofGetHeight() * 0.45, 0));
				}
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWindowSize() * 0.5);

	int span = 20;
	for (auto& location : this->location_list) {

		auto x = location.x;
		auto y = location.y;

		auto p = ofGetFrameNum() % 30;
		auto next_p = (ofGetFrameNum() + 1) % 30;

		//auto param = p < 5 ? 0 : p < 15 ? ofMap(p, 5, 15, 0, 1) : p < 25 ? 1 : ofMap(p, 25, 30, 1, 0);
		
		auto param = p < 5 ? 0 : p < 15 ? ofMap(p, 5, 15, 0, 1) : p < 25 ? 1 : ofMap(p, 25, 30, 1, 0);
		auto next_param = next_p < 5 ? 0 : next_p < 15 ? ofMap(next_p, 5, 15, 0, 1) : next_p < 25 ? 1 : ofMap(next_p, 25, 30, 1, 0);

		auto noise_seed = glm::vec2(ofRandom(1000), ofRandom(1000));

		auto param_x = ofMap(ofNoise(noise_seed.x, ofGetFrameNum() * 0.01), 0, 1, span * -5, span * 5) * param;
		auto param_y = ofMap(ofNoise(noise_seed.y, ofGetFrameNum() * 0.01), 0, 1, span * -5, span * 5) * param;

		auto next_param_x = ofMap(ofNoise(noise_seed.x, (ofGetFrameNum() + 1) * 0.01), 0, 1, span * -5, span * 5) * next_param;
		auto next_param_y = ofMap(ofNoise(noise_seed.y, (ofGetFrameNum() + 1) * 0.01), 0, 1, span * -5, span * 5) * next_param;

		if (param > 0) {

			this->draw_arrow(glm::vec2(x + param_x, y + param_y), glm::vec2(x + next_param_x, y + next_param_y), span, ofColor(0), ofColor(255));
		}
		else {

			ofSetColor(0);
			ofFill();
			ofDrawCircle(x + param_x, y + param_y, span * 0.35);
			
			ofSetColor(255);
			ofNoFill();
			ofDrawCircle(x + param_x, y + param_y, span * 0.35);
		}
	}

	/*
	ofTranslate(ofGetWindowSize() * 0.5);

	int span = 10;
	for (int x = -300; x <= 300; x += span) {

		for (int y = -300; y <= 300; y += span) {

			auto noise_y = ofMap(ofNoise(x * 0.0025 + ofGetFrameNum() * 0.035), 0, 1, -400, 400);
			auto next_noise_y = ofMap(ofNoise(x * 0.0025 + (ofGetFrameNum() + 1) * 0.035), 0, 1, -400, 400);

			auto noise_param = abs(y - noise_y) < 125 ? ofMap(abs(y - noise_y), 0, 125, 1, 0) : 0;
			auto next_noise_param = abs(y - next_noise_y) < 125 ? ofMap(abs(y - next_noise_y), 0, 125, 1, 0) : 0;

			auto noise_seed = glm::vec2(ofRandom(1000), ofRandom(1000));

			auto param_x = ofMap(ofNoise(noise_seed.x, ofGetFrameNum() * 0.05), 0, 1, span * -1.25, span * 1.25) * noise_param;
			auto param_y = ofMap(ofNoise(noise_seed.y, ofGetFrameNum() * 0.05), 0, 1, span * -1.25, span * 1.25) * noise_param;

			auto next_param_x = ofMap(ofNoise(noise_seed.x, (ofGetFrameNum() + 1) * 0.05), 0, 1, span * -1.25, span * 1.25) * next_noise_param;
			auto next_param_y = ofMap(ofNoise(noise_seed.y, (ofGetFrameNum() + 1) * 0.05), 0, 1, span * -1.25, span * 1.25) * next_noise_param;

			if (noise_param > 0) {

				this->draw_arrow(glm::vec2(x + param_x, y + param_y), glm::vec2(x + next_param_x, y + next_param_y), span * 0.5, ofColor(0), ofColor(255));
			}
			else {

				ofSetColor(0);
				ofFill();
				ofDrawCircle(x + param_x, y + param_y, span * 0.2);

				ofSetColor(255);
				ofNoFill();
				ofDrawCircle(x + param_x, y + param_y, span * 0.2);
			}
		}
	}
	*/
}

//--------------------------------------------------------------
void ofApp::draw_arrow(glm::vec2 location, glm::vec2 next_location, float size, ofColor fill_color, ofColor no_fill_color) {

	auto angle = std::atan2(next_location.y - location.y, next_location.x - location.x);

	ofSetColor(fill_color);
	ofFill();
	ofBeginShape();
	ofVertex(glm::vec2(size * 0.8 * cos(angle), size * 0.8 * sin(angle)) + location);
	ofVertex(glm::vec2(size * 0.5 * cos(angle + PI * 0.5), size * 0.5 * sin(angle + PI * 0.5)) + location);
	ofVertex(glm::vec2(size * 0.5 * cos(angle - PI * 0.5), size * 0.5 * sin(angle - PI * 0.5)) + location);
	ofEndShape(true);

	ofBeginShape();
	ofVertex(glm::vec2(size * 0.5 * cos(angle + PI * 0.5), size * 0.5 * sin(angle + PI * 0.5)) * 0.25 + location);
	ofVertex(glm::vec2(size * 0.5 * cos(angle + PI * 0.5), size * 0.5 * sin(angle + PI * 0.5)) * 0.25 - glm::vec2(size * cos(angle), size * sin(angle)) * 0.5 + location);
	ofVertex(glm::vec2(size * 0.5 * cos(angle - PI * 0.5), size * 0.5 * sin(angle - PI * 0.5)) * 0.25 - glm::vec2(size * cos(angle), size * sin(angle)) * 0.5 + location);
	ofVertex(glm::vec2(size * 0.5 * cos(angle - PI * 0.5), size * 0.5 * sin(angle - PI * 0.5)) * 0.25 + location);
	ofEndShape(true);

	ofSetColor(no_fill_color);
	ofNoFill();
	ofBeginShape();
	ofVertex(glm::vec2(size * 0.5 * cos(angle + PI * 0.5), size * 0.5 * sin(angle + PI * 0.5)) * 0.25 + location);
	ofVertex(glm::vec2(size * 0.5 * cos(angle + PI * 0.5), size * 0.5 * sin(angle + PI * 0.5)) + location);
	ofVertex(glm::vec2(size * 0.8 * cos(angle), size * 0.8 * sin(angle)) + location);
	ofVertex(glm::vec2(size * 0.5 * cos(angle - PI * 0.5), size * 0.5 * sin(angle - PI * 0.5)) + location);
	ofVertex(glm::vec2(size * 0.5 * cos(angle - PI * 0.5), size * 0.5 * sin(angle - PI * 0.5)) * 0.25 + location);
	ofEndShape();

	ofBeginShape();
	ofVertex(glm::vec2(size * 0.5 * cos(angle + PI * 0.5), size * 0.5 * sin(angle + PI * 0.5)) * 0.25 + location);
	ofVertex(glm::vec2(size * 0.5 * cos(angle + PI * 0.5), size * 0.5 * sin(angle + PI * 0.5)) * 0.25 - glm::vec2(size * cos(angle), size * sin(angle)) * 0.5 + location);
	ofVertex(glm::vec2(size * 0.5 * cos(angle - PI * 0.5), size * 0.5 * sin(angle - PI * 0.5)) * 0.25 - glm::vec2(size * cos(angle), size * sin(angle)) * 0.5 + location);
	ofVertex(glm::vec2(size * 0.5 * cos(angle - PI * 0.5), size * 0.5 * sin(angle - PI * 0.5)) * 0.25 + location);
	ofEndShape();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}