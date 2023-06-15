import { Injectable } from '@angular/core';
import { HttpClient, HttpHeaders } from '@angular/common/http';
import { Observable, take } from 'rxjs';
import { environment } from 'src/environments/environment';

@Injectable({
  providedIn: 'root'
})
export class DnsService {

  constructor(private httpClient: HttpClient) { }

  getAll(){
    return this.httpClient
    .get("/api/getAll")
    .pipe(take(1))
  }

}


