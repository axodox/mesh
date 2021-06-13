import { Component } from '@angular/core';
import { HttpClient } from '@angular/common/http';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss']
})
export class AppComponent {
  title = 'axodox-mesh';

  private ledState : boolean = false;

  constructor(private http: HttpClient) {

  }

  async OnClick() {
    console.log("I am clicked!");

    if(this.ledState)
    {
      await this.http.put<string>("api/led/off", null).toPromise();
    }
    else
    {
      await this.http.put<string>("api/led/on", null).toPromise();
    }
    this.ledState = !this.ledState;
  }
}
