import { Component, Input } from '@angular/core';
import { AnyLightSourceSettings, UartSourceSettings } from 'src/app/data/light-strip-settings';
import { LightStripService } from 'src/app/services/light-strip-service';

@Component({
  selector: 'app-uart-light-source',
  templateUrl: './uart-light-source.component.html',
  styleUrls: ['./uart-light-source.component.scss']
})
export class UartLightSourceComponent {

  @Input() source: AnyLightSourceSettings; 

  constructor(
    private readonly lightStripService : LightStripService
  ) { }

  ngOnInit(): void {
  }

  onBaudRateChange(event: any) {
    this.source.baudRate = parseFloat(event.target.value);
    
    let settings = new UartSourceSettings();
    settings.baudRate = this.source.baudRate;
    this.lightStripService.setLightSourceSettings(settings);
  }
}