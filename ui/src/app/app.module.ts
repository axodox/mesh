import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';
import { HttpClientModule } from '@angular/common/http'; 

import { AppComponent } from './app.component';
import { LightStripComponent } from './Components/light-strip/light-strip.component';

@NgModule({
  declarations: [
    AppComponent,
    LightStripComponent
  ],
  imports: [
    BrowserModule,
    HttpClientModule,
    HttpClientModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
