import { HttpClient } from "@angular/common/http";
import { Injectable, OnDestroy } from "@angular/core";
import { AnyLightSourceSettings, BrightnessProcessorSettings, DeviceSettings, LightSourceType, EmptySourceSettings } from "../data/light-strip-settings";
import { CallLimiter } from "../framework/call-limiter";

@Injectable({
  providedIn: 'root'
})
export class LightStripService implements OnDestroy {
  private readonly urlBase = '/api/light_strip/';
  private webSocket: WebSocket | any;
  private connection: Promise<void> | any;
  private isActive = false;

  constructor(
    private http: HttpClient) {

    this.isActive = true;
    this.connection = new Promise<void>(async (resolve) => {
      const url = `ws://${window.location.host}/ws/light_strip`;
      
      while(this.isActive) {
        try {
          console.log(`connecting to ${url}...`);
          let webSocket = new WebSocket(url);
          this.webSocket = webSocket;

          webSocket.onmessage = (event) => { };

          await new Promise<void>(p => webSocket.onopen = (event) => p());

          await new Promise<void>(p => {
            webSocket.onerror = (event) => p();
            webSocket.onclose = (event) => p();
          });

          this.webSocket = null;
        } catch (exception) {
          console.log(exception);
          await new Promise(p => setTimeout(p, 1000));
        }
      }
    });
  }

  private readonly brightnessCallLimiter = new CallLimiter<BrightnessProcessorSettings>(p =>  this.http.post(this.urlBase + "brightness", p).toPromise());
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
    if(this.webSocket) {
      (this.webSocket as WebSocket).send(JSON.stringify(value));
    }
    else {
      this.brightnessCallLimiter.call(value);
    }
  }

  getLightSourceSettings() {
    return this.http.get<AnyLightSourceSettings>(this.urlBase + "mode").toPromise();
  }

  switchLightSource(value: LightSourceType) {
    let source = new EmptySourceSettings();
    source.$type = value;
    return this.http.post<AnyLightSourceSettings>(this.urlBase + "mode", source).toPromise();
  }

  setLightSourceSettings(value: AnyLightSourceSettings) {
    this.sourceCallLimiter.call(value);
  }

  async ngOnDestroy() {
    if (!this.isActive) return;

    this.isActive = false;
    if(this.webSocket) {
      this.webSocket.close();
    }
    if (this.connection) {
      await this.connection;
    }
  }
}