export class CallLimiter<T> {
  private nextArg: T | null = null;
  private callInProgress = false;

  constructor(
    private readonly callback: (arg: T) => Promise<any>) {
  }

  call(arg: T) {
    this.nextArg = JSON.parse(JSON.stringify(arg));

    if(!this.callInProgress) setTimeout(() => this.run());
  }

  private async run() {
    let arg = this.nextArg;
    this.nextArg = null;

    if(arg == null) return;

    try {
      this.callInProgress = true;
      await this.callback(arg);
    } 
    catch(e) {
      console.log(e);
    }
    finally {
      this.callInProgress = false;
    }

    setTimeout(() => this.run());
  }
}