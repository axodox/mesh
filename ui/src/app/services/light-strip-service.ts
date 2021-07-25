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

  GetDeviceSettings() {
    return this.http.get<DeviceSettings>(this.urlBase + "device");
  }

  SetDeviceSettings(value: DeviceSettings) {
    return this.http.post<DeviceSettings>(this.urlBase + "device", value);
  }

  GetBrightnessSettings() {
    return this.http.get<BrightnessProcessorSettings>(this.urlBase + "brightness");
  }

  SetBrightnessSettings(value: BrightnessProcessorSettings) {
    return this.http.post<BrightnessProcessorSettings>(this.urlBase + "brightness", value);
  }

  GetLightSourceSettings() {
    return this.http.get<AnyLightSourceSettings>(this.urlBase + "source");
  }

  SetLightSourceSettings(value: AnyLightSourceSettings) {
    return this.http.post<AnyLightSourceSettings>(this.urlBase + "source", value);
  }
}