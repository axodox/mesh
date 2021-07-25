export class ColorRgb {
  constructor(
    public r = 0,
    public g = 0,
    public b = 0) {
  }
}

export class Float3 {
  constructor(
    public x = 0,
    public y = 0,
    public z = 0) {
  }
}

export class BrightnessProcessorSettings {
  gamma = new Float3(1.6, 1.5, 1.6);
  brightness = 1;
  maxBrightness = 0.7;
}

export class DeviceSettings {
  lightCount = 4;
  interval = 16; 
}

export enum LightSourceType {
  None = "none",
  Static = "static",
  Rainbow = "rainbow"
}

export type AnyLightSourceSettings = NoneSourceSettings | StaticSourceSettings | RainbowSourceSettings

export abstract class LightSourceSettings {
  $type: LightSourceType | undefined;
}

export class NoneSourceSettings extends LightSourceSettings {
  $type = LightSourceType.None;
}

export class StaticSourceSettings extends LightSourceSettings {
  $type = LightSourceType.Static;
  color = new ColorRgb();
}

export class RainbowSourceSettings extends LightSourceSettings {
  $type = LightSourceType.Rainbow;
  spatial_frequency = 1;
  angular_velocity = Math.PI;
}