import { HttpClient } from "@angular/common/http";
import { Injectable } from "@angular/core";
import { AnyLightSourceSettings, BrightnessProcessorSettings, DeviceSettings, LightSourceType, NoneSourceSettings } from "../data/light-strip-settings";
import { CallLimiter } from "../framework/call-limiter";

@Injectable({
  providedIn: 'root'
})
export class LightStripService {
  private readonly urlBase = '/api/light_strip/';
  
  constructor(
    private http: HttpClient) {
  }

  private readonly brightnessCallLimiter = new CallLimiter<BrightnessProcessorSettings>(p => this.http.post(this.urlBase + "brightness", p).toPromise());
  private readonly sourceCallLimiter = new CallLimiter<AnyLightSourceSettings>(p => this.http.post(this.urlBase + "mode", p).toPromise());

  getDeviceSettings() {
    return this.http.get<DeviceSettings>(this.urlBase + "device").toPromise();
  }

  setDeviceSettings(value: DeviceSettings) {
    return this.http.post<DeviceSettings>(this.urlBase + "device", value).toPromise();
  }

  getBrightnessSettings() {
    return this.http.get<BrightnessProcessorSettings>(this.urlBase + "brightness").toPromise();
  }

  setBrightnessSettings(value: BrightnessProcessorSettings) {
    this.brightnessCallLimiter.call(value);
  }

  getLightSourceSettings() {
    return this.http.get<AnyLightSourceSettings>(this.urlBase + "mode").toPromise();
  }

  switchLightSource(value: LightSourceType) {
    let source = new NoneSourceSettings();
    source.$type = value;
    return this.http.post<AnyLightSourceSettings>(this.urlBase + "mode", source).toPromise();
  }

  setLightSourceSettings(value: AnyLightSourceSettings) {
    this.sourceCallLimiter.call(value);
  }
}