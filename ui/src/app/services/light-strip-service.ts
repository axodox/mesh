import { HttpClient } from "@angular/common/http";
import { Injectable } from "@angular/core";
import { AnyLightSourceSettings, BrightnessProcessorSettings, DeviceSettings } from "../data/light-strip-settings";

@Injectable({
  providedIn: 'root'
})
export class LightStripService {
  private readonly urlBase = '/api/light_strip/';

  constructor(
    private http: HttpClient) {
  }

  getDeviceSettings() {
    return this.http.get<DeviceSettings>(this.urlBase + "device");
  }

  setDeviceSettings(value: DeviceSettings) {
    return this.http.post<DeviceSettings>(this.urlBase + "device", value);
  }

  getBrightnessSettings() {
    return this.http.get<BrightnessProcessorSettings>(this.urlBase + "brightness");
  }

  setBrightnessSettings(value: BrightnessProcessorSettings) {
    return this.http.post<BrightnessProcessorSettings>(this.urlBase + "brightness", value);
  }

  getLightSourceSettings() {
    return this.http.get<AnyLightSourceSettings>(this.urlBase + "mode");
  }

  setLightSourceSettings(value: AnyLightSourceSettings) {
    return this.http.post<AnyLightSourceSettings>(this.urlBase + "mode", value);
  }
}