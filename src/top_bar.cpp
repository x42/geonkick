/**
 * File name: top_bar.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2018 Iurie Nistor (http://geontime.com)
 *
 * This file is part of Geonkick.
 *
 * GeonKick is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "top_bar.h"
#include "geonkick_button.h"
#include "preset_browser_model.h"
#include "preset_browser_view.h"

#include <RkLabel.h>
#include <RkButton.h>
#include <RkTransition.h>

RK_DECLARE_IMAGE_RC(logo);
RK_DECLARE_IMAGE_RC(open_active);
RK_DECLARE_IMAGE_RC(save_active);
RK_DECLARE_IMAGE_RC(export_active);
RK_DECLARE_IMAGE_RC(presets_button);
RK_DECLARE_IMAGE_RC(play);
RK_DECLARE_IMAGE_RC(play_pressed);
RK_DECLARE_IMAGE_RC(topbar_layer1);
RK_DECLARE_IMAGE_RC(topbar_layer2);
RK_DECLARE_IMAGE_RC(topbar_layer3);
RK_DECLARE_IMAGE_RC(topbar_layer1_disabled);
RK_DECLARE_IMAGE_RC(topbar_layer2_disabled);
RK_DECLARE_IMAGE_RC(topbar_layer3_disabled);
RK_DECLARE_IMAGE_RC(tune_checkbox_on);
RK_DECLARE_IMAGE_RC(tune_checkbox_off);

TopBar::TopBar(GeonkickWidget *parent, GeonkickApi *api)
        : GeonkickWidget(parent)
        , openFileButton{nullptr}
        , saveFileButton{nullptr}
        , exportFileButton{nullptr}
        , presetNameLabel{nullptr}
        , layer1Button{nullptr}
        , layer2Button{nullptr}
        , layer3Button{nullptr}
        , geonkickApi{api}
        , tuneCheckbox{nullptr}
        , presetsModel{new PresetBrowserModel(this, api)}
{
        setFixedWidth(parent->width());
        setFixedHeight(40);

        auto logo = new RkLabel(this);
        RkImage image(120, 20, RK_IMAGE_RC(logo));
        logo->setSize(image.width(), image.height());
        logo->setBackgroundColor(68, 68, 70);
        logo->setImage(image);
        logo->setY((height() - logo->height()) / 2);
        logo->show();

        openFileButton = new GeonkickButton(this);
        openFileButton->show();
        openFileButton->setSize(90, 30);
        openFileButton->setX(logo->x() + logo->width() + 5);
        openFileButton->setY((height() - openFileButton->height()) / 2);
        openFileButton->setUnpressedImage(RkImage(90, 30, RK_IMAGE_RC(open_active)));
        openFileButton->setCheckable(true);
        RK_ACT_BIND(openFileButton, pressed, RK_ACT_ARGS(), this, openFile());

        saveFileButton = new GeonkickButton(this);
        saveFileButton->setSize(90, 30);
        saveFileButton->setX(openFileButton->x() + openFileButton->width() + 5);
        saveFileButton->setY(openFileButton->y());
        saveFileButton->setUnpressedImage(RkImage(90, 30, RK_IMAGE_RC(save_active)));
        saveFileButton->setCheckable(true);
        RK_ACT_BIND(saveFileButton, pressed, RK_ACT_ARGS(), this, saveFile());

        exportFileButton = new GeonkickButton(this);
        exportFileButton->setSize(90, 30);
        exportFileButton->setX(saveFileButton->x() + saveFileButton->width() + 5);
        exportFileButton->setY(saveFileButton->y());
        exportFileButton->setUnpressedImage(RkImage(90, 30, RK_IMAGE_RC(export_active)));
        exportFileButton->setCheckable(true);
        RK_ACT_BIND(exportFileButton, pressed, RK_ACT_ARGS(), this, openExport());

        auto presetsButton = new RkButton(this);
        presetsButton->setType(RkButton::ButtonType::ButtonPush);
        presetsButton->setSize(90, 30);
        presetsButton->setX(exportFileButton->x() + exportFileButton->width() + 5);
        presetsButton->setY(exportFileButton->y());
        presetsButton->setImage(RkImage(90, 30, RK_IMAGE_RC(presets_button)));
        presetsButton->show();
        RK_ACT_BIND(presetsButton, pressed, RK_ACT_ARGS(), this, showPresetsBrowser());

        presetNameLabel = new RkLabel(this);
        presetNameLabel->setBackgroundColor(background());
        presetNameLabel->setTextColor({210, 226, 226, 140});
        auto font = presetNameLabel->font();
        font.setSize(12);
        presetNameLabel->setFont(font);
        presetNameLabel->setSize(220, 30);
        presetNameLabel->setPosition(presetsButton->x() + presetsButton->width() + 5,
                                     (height() - presetNameLabel->height()) / 2);
        presetNameLabel->show();

	auto playButton = new RkButton(this);
        playButton->setType(RkButton::ButtonType::ButtonPush);
        playButton->setSize(24, 24);
        playButton->setX(presetNameLabel->x() + presetNameLabel->width() + 10);
        playButton->setY((height() - playButton->height()) / 2);
        playButton->setImage(RkImage(playButton->size(), RK_IMAGE_RC(play)),
                             RkButton::ButtonImage::ImageUnpressed);
        playButton->setImage(RkImage(playButton->size(), RK_IMAGE_RC(play_pressed)),
                             RkButton::ButtonImage::ImagePressed);
        RK_ACT_BIND(playButton, pressed, RK_ACT_ARGS(), geonkickApi, playKick());
	playButton->show();

        createLyersButtons();

        tuneCheckbox = new GeonkickButton(this);
        tuneCheckbox->setCheckable(true);
        tuneCheckbox->setFixedSize(46, 11);
        tuneCheckbox->setPosition(width() - tuneCheckbox->width() - 30, (height() - tuneCheckbox->height()) / 2);
        tuneCheckbox->setPressedImage(RkImage(tuneCheckbox->size(), RK_IMAGE_RC(tune_checkbox_on)));
        tuneCheckbox->setUnpressedImage(RkImage(tuneCheckbox->size(), RK_IMAGE_RC(tune_checkbox_off)));
        tuneCheckbox->show();
        RK_ACT_BIND(tuneCheckbox, toggled, RK_ACT_ARGS(bool b), geonkickApi,
		    tuneAudioOutput(geonkickApi->currentPercussion(), b));

        updateGui();
}

void TopBar::createLyersButtons()
{
        int layersX = width() - 180;
        int layersSpace = 5;
        layer1Button = new GeonkickButton(this);
        layer1Button->setBackgroundColor(background());
        layer1Button->setSize(24, 24);
        layer1Button->setPosition(layersX, (height() - layer1Button->height()) / 2);
        layer1Button->setUnpressedImage(RkImage(layer1Button->size(), RK_IMAGE_RC(topbar_layer1_disabled)));
        layer1Button->setPressedImage(RkImage(layer1Button->size(), RK_IMAGE_RC(topbar_layer1)));
        layer1Button->setCheckable(true);

        layer2Button = new GeonkickButton(this);
        layer2Button->setBackgroundColor(background());
        layer2Button->setSize(24, 24);
        layer2Button->setPosition(layer1Button->x() + layer1Button->width() + layersSpace, layer1Button->y());
	layer2Button->setUnpressedImage(RkImage(layer2Button->size(), RK_IMAGE_RC(topbar_layer2_disabled)));
	layer2Button->setPressedImage(RkImage(layer2Button->size(), RK_IMAGE_RC(topbar_layer2)));
        layer2Button->setCheckable(true);

        layer3Button = new GeonkickButton(this);
        layer3Button->setBackgroundColor(background());
        layer3Button->setSize(24, 24);
        layer3Button->setPosition(layer2Button->x() + layer2Button->width() + layersSpace, layer2Button->y());
        layer3Button->setUnpressedImage(RkImage(layer3Button->size(), RK_IMAGE_RC(topbar_layer3_disabled)));
        layer3Button->setPressedImage(RkImage(layer3Button->size(), RK_IMAGE_RC(topbar_layer3)));
        layer3Button->setCheckable(true);

        RK_ACT_BIND(layer1Button, toggled, RK_ACT_ARGS(bool b),
                    geonkickApi, enbaleLayer(GeonkickApi::Layer::Layer1, b));
        RK_ACT_BIND(layer3Button, toggled, RK_ACT_ARGS(bool b),
                    geonkickApi, enbaleLayer(GeonkickApi::Layer::Layer3, b));
        RK_ACT_BIND(layer2Button, toggled, RK_ACT_ARGS(bool b),
                    geonkickApi, enbaleLayer(GeonkickApi::Layer::Layer2, b));
}

void TopBar::setPresetName(const std::string &name)
{
        if (name.size() > 20) {
                std::string preset = name;
                preset.resize(15);
                preset += "...";
                presetNameLabel->setText(preset);
        } else {
                presetNameLabel->setText(name);
        }
}

void TopBar::updateGui()
{
        layer1Button->setPressed(geonkickApi->isLayerEnabled(GeonkickApi::Layer::Layer1));
        layer2Button->setPressed(geonkickApi->isLayerEnabled(GeonkickApi::Layer::Layer2));
        layer3Button->setPressed(geonkickApi->isLayerEnabled(GeonkickApi::Layer::Layer3));
        tuneCheckbox->setPressed(geonkickApi->isAudioOutputTuned(geonkickApi->currentPercussion()));
        setPresetName(geonkickApi->getPercussionName(geonkickApi->currentPercussion()));
}

void TopBar::showPresetsBrowser()
{
        auto presetBrowser = new PresetBrowserView(this, presetsModel);
        presetBrowser->setPosition(50, height() + 360);
        presetBrowser->setBackgroundColor({68, 68, 70});
        presetBrowser->show();
}
