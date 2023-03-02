#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SSPApiEditor.h"

//TODO
// - PluginEditorInterface::draw  ... I don't use this, not sure when it would be needed

PluginEditor::PluginEditor(PluginProcessor &p, unsigned maxModules)
    : base_type(&p),
      processor_(p), maxModules_(maxModules) {
    setSize(1600, 480);
    setSize(1600, 480);
}


void PluginEditor::drawView(Graphics &g) {
    drawControls(g);
    for (int i = 0; i < maxModules_; i++) {
        drawPanel(g, i);
    }
}

void PluginEditor::drawControls(Graphics &g) {
}


void PluginEditor::drawPanel(Graphics &g, unsigned panel) {
    auto editor = processor_.getEditor(panel);
    if (!editor) return;

#define IMAGECACHE_HASHCODE 0x53533500000

    // draw IO
    drawIO(g, panel);

    // TODO
    Image img = ImageCache::getFromHashCode(IMAGECACHE_HASHCODE + panel);
    if (!img.isValid()) {
        Image newimg(Image::ARGB, pluginWidth, pluginHeight, true);
        ImageCache::addImageToCache(newimg, IMAGECACHE_HASHCODE + panel);
        img = newimg;
    }

    Image::BitmapData bitmap(img, Image::BitmapData::writeOnly);
    editor->renderToImage(bitmap.data, pluginWidth, pluginHeight);

    // TODO
    g.drawImageAt(img, panel * 640, 0);
}



void PluginEditor::drawIO(Graphics &g, unsigned panel) {
}

void PluginEditor::selectPlugin(Graphics &g, unsigned panel) {
}



void PluginEditor::onSSPTimer() {
    for (int i = 0; i < maxModules_; i++) {
        auto editor = processor_.getEditor(i);
        if (!editor) continue;
        editor->frameStart();
    }
}

void PluginEditor::editorShown() {
    for (int i = 0; i < maxModules_; i++) {
        auto editor = processor_.getEditor(i);
        if (!editor) continue;
        editor->visibilityChanged(true);
    }
}

void PluginEditor::editorHidden() {
    for (int i = 0; i < maxModules_; i++) {
        auto editor = processor_.getEditor(i);
        if (!editor) continue;
        editor->visibilityChanged(false);
    }
}


void PluginEditor::onEncoder(unsigned enc, float v) {
    auto plugin = processor_.getPlugin(activeModule_);
    if (!plugin) return;
    plugin->encoderTurned(enc, v);
}

void PluginEditor::onEncoderSwitch(unsigned enc, bool v) {
    auto plugin = processor_.getPlugin(activeModule_);
    if (!plugin) return;
    plugin->encoderPressed(enc, v);
}

void PluginEditor::onButton(unsigned btn, bool v) {
    auto plugin = processor_.getPlugin(activeModule_);
    if (!plugin) return;
    plugin->buttonPressed(btn, v);
}

void PluginEditor::onLeftButton(bool v) {
    if (!v) activeModule_ = 0;
}

void PluginEditor::onRightButton(bool v) {
    if (!v) activeModule_ = 1;
}

void PluginEditor::onUpButton(bool v) {
}

void PluginEditor::onDownButton(bool v) {
}

void PluginEditor::onLeftShiftButton(bool v) {
    if (v) viewMode_[0] = (eViewMode) !viewMode_[0];
}

void PluginEditor::onRightShiftButton(bool v) {
    if (v) viewMode_[1] = (eViewMode) !viewMode_[1];
}


void PluginEditor::resized() {
    base_type::resized();
    for (int i = 0; i < maxModules_; i++) {
        auto editor = processor_.getEditor(i);
        if (!editor) continue;
        editor->draw(pluginWidth,pluginHeight);
    }
}
