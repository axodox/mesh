import { Component, Input } from '@angular/core';
import { AnyLightSourceSettings, UdpSourceSettings } from 'src/app/data/light-strip-settings';
import { LightStripService } from 'src/app/services/light-strip-service';

@Component({
  selector: 'app-udp-light-source',
  templateUrl: './udp-light-source.component.html',
  styleUrls: ['./udp-light-source.component.scss']
})
export class UdpLightSourceComponent {

  @Input() source: AnyLightSourceSettings; 

  constructor(
    private readonly lightStripService : LightStripService
  ) { }

  onPortChange(event: any) {
    this.source.port = parseFloat(event.target.value);
    
    let settings = new UdpSourceSettings();
    settings.port = this.source.port;
    this.lightStripService.setLightSourceSettings(settings);
  }
}
