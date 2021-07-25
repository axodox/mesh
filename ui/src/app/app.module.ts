import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';
import { HttpClientModule } from '@angular/common/http'; 

import { AppComponent } from './app.component';
import { LightStripComponent } from './components/light-strip/light-strip.component';
import { FormsModule } from '@angular/forms';
import { StaticLightSourceComponent } from './components/static-light-source/static-light-source.component';

@NgModule({
  declarations: [
    AppComponent,
    LightStripComponent,
    StaticLightSourceComponent
  ],
  imports: [
    BrowserModule,
    FormsModule,
    HttpClientModule,
    HttpClientModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
