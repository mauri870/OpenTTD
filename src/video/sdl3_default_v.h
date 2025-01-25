/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file sdl3_default_v.h Default backend of the SDL3 video driver. */

#ifndef VIDEO_SDL3_DEFAULT_H
#define VIDEO_SDL3_DEFAULT_H

#include "sdl3_v.h"

/** The SDL3 video driver using default SDL3 backend. */
class VideoDriver_SDL3_Default : public VideoDriver_SDL3_Base {
public:
	std::string_view GetName() const override { return "sdl3"; }

protected:
	bool AllocateBackingStore(int w, int h, bool force = false) override;
	void *GetVideoPointer() override;
	void Paint() override;

	void ReleaseVideoPointer() override {}

private:
	void UpdatePalette();
	void MakePalette();
};

/** Factory for the SDL3 video driver. */
class FVideoDriver_SDL3_Default : public DriverFactoryBase {
public:
	FVideoDriver_SDL3_Default() : DriverFactoryBase(Driver::DT_VIDEO, 5, "sdl3", "SDL3 Video Driver") {}
	Driver *CreateInstance() const override { return new VideoDriver_SDL3_Default(); }
};

#endif /* VIDEO_SDL3_DEFAULT_H */
