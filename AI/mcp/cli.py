import argparse
import os
from server import client


def main():
    parser = argparse.ArgumentParser(description="MCP operator CLI")
    parser.add_argument("--api-key", default=os.getenv("MCP_API_KEY", "secret"))
    parser.add_argument("--tenant", default=None, help="tenant id for JWT auth")
    sub = parser.add_subparsers(dest="command")

    reg = sub.add_parser("register")
    reg.add_argument("name")
    reg.add_argument("version")
    reg.add_argument("model_url")
    reg.add_argument("model_card_url")

    listp = sub.add_parser("list")
    listp.add_argument("--name", default=None)
    listp.add_argument("--tag", default=None, help="filter by tag")

    prom = sub.add_parser("promote")
    prom.add_argument("name")
    prom.add_argument("version")
    prom.add_argument("target")
    prom.add_argument("--canary", type=int, default=None)

    searchp = sub.add_parser("search")
    searchp.add_argument("query")


    rollback = sub.add_parser("rollback")
    rollback.add_argument("name")
    rollback.add_argument("to_version")

    metrics = sub.add_parser("metrics")
    metrics.add_argument("model_name")
    metrics.add_argument("model_version")
    metrics.add_argument("--metric", action="append", nargs=2, metavar=("KEY", "VALUE"))

    args = parser.parse_args()
    cli = client.MPCClient(api_key=args.api_key)
    if args.tenant:
        cli._api_key = args.tenant  # for JWT this would typically be the token

    if args.command == "register":
        payload = {
            "name": args.name,
            "version": args.version,
            "model_url": args.model_url,
            "model_card_url": args.model_card_url,
        }
        print(cli.register(payload))
    elif args.command == "list":
        print(cli.list_models(name=args.name, tag=args.tag))
    elif args.command == "search":
        print(cli.search_models(args.query))
    elif args.command == "promote":
        payload = {"name": args.name, "version": args.version, "target": args.target}
        if args.canary is not None:
            payload["canary_percent"] = args.canary
        print(cli.promote(payload))
    elif args.command == "rollback":
        print(cli.rollback({"name": args.name, "to_version": args.to_version}))
    elif args.command == "metrics":
        metrics_dict = {k: float(v) for k, v in args.metric} if args.metric else {}
        print(cli.metrics({
            "model_name": args.model_name,
            "model_version": args.model_version,
            "metrics": metrics_dict,
        }))
    else:
        parser.print_help()


if __name__ == "__main__":
    main()
