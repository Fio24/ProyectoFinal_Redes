import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { DNSRegitsterFormComponent } from './dns-regitster-form/dns-regitster-form.component';
import { HomeComponent } from './home/home.component';
import { HttpClient, HttpClientModule } from '@angular/common/http';
import { FormsModule } from '@angular/forms';
import { DnsEditFormComponent } from './dns-edit-form/dns-edit-form.component';

@NgModule({
  declarations: [
    AppComponent,
    DNSRegitsterFormComponent,
    HomeComponent,
    DnsEditFormComponent
  ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    HttpClientModule,
    FormsModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
